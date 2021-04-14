/*==========================================================================;
 *
 *  Copyright (C) 1995 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       d3dmem.c
 *  Content:    Direct3D mem allocation
 *@@BEGIN_MSINTERNAL
 *
 *  History:
 *   Date       By      Reason
 *   ====       ==      ======
 *   10/12/95   stevela Initial rev with this header.
 *@@END_MSINTERNAL
 *
 ***************************************************************************/

#include "pch.cpp"
#pragma hdrstop

static D3DMALLOCFUNCTION malloc_function = (D3DMALLOCFUNCTION) MemAlloc;
static D3DREALLOCFUNCTION realloc_function = (D3DREALLOCFUNCTION) MemReAlloc;
static D3DFREEFUNCTION free_function = MemFree;

#undef DPF_MODNAME
#define DPF_MODNAME "D3DMalloc"

HRESULT D3DAPI D3DMalloc(LPVOID* p_return, size_t size)
{
    void* p;

    if (!VALID_OUTPTR(p_return)) {
        D3D_ERR("Bad pointer given");
        return DDERR_OUTOFMEMORY;
    }
    if (size > 0) {
        p = malloc_function(size);
        if (p == NULL)
            return (DDERR_OUTOFMEMORY);
    } else {
        p = NULL;
    }
    *p_return = p;

    return (D3D_OK);
}

#undef DPF_MODNAME
#define DPF_MODNAME "D3DRealloc"

HRESULT D3DAPI D3DRealloc(LPVOID* p_inout, size_t size)
{
    void* p = *p_inout;
    HRESULT err = D3D_OK;

    if (!VALID_OUTPTR(p_inout)) {
        D3D_ERR("Bad pointer given");
        return DDERR_OUTOFMEMORY;
    }
    if (size > 0) {
        if (p) {
            p = realloc_function(p, size);
            if (p == NULL)
                return (DDERR_OUTOFMEMORY);
        } else
            return D3DMalloc(p_inout, size);
    } else if (size == 0) {
        D3DFree(p);
        p = NULL;
    } else
        return (DDERR_INVALIDPARAMS);
    *p_inout = p;
    return (err);
}

#undef DPF_MODNAME
#define DPF_MODNAME "D3DFree"

VOID D3DAPI D3DFree(LPVOID p)
{
    if (p == NULL) return;

    if (!VALID_DWORD_PTR(p)) {
        D3D_ERR("invalid pointer");
        return;
    }
    if (p) {
        free_function(p);
    }
}

#define CACHE_LINE 32
HRESULT MallocAligned(void** p_return, size_t size)
{
    char* p;
    size_t offset;
    HRESULT error;

    if (!p_return)
        return DDERR_INVALIDPARAMS;

    if (size > 0) {
        if ((error = D3DMalloc((void**) &p, size + CACHE_LINE)) != DD_OK)
        {
            *p_return = NULL;
            return error;
        }
        offset = (size_t)(CACHE_LINE - ((ULONG_PTR)p & (CACHE_LINE - 1)));
        p += offset;
        ((size_t*)p)[-1] = offset;
    } else
        p = NULL;
    *p_return = p;
    return DD_OK;
}

void FreeAligned(void* p)
{
    if (p) {
        size_t offset = ((size_t*)p)[-1];
        p = (void*) ((unsigned char*)p - offset);
        D3DFree(p);
    }
}

HRESULT ReallocAligned(void** p_inout, size_t size)
{
    char* p = (char*)*p_inout;
    HRESULT error;

    if (!p_inout)
        return DDERR_INVALIDPARAMS;

    if (size > 0) {
        if (p) {
            size_t old_offset = ((size_t*)p)[-1];
            size_t new_offset;

            p -= old_offset;
            if ((error = D3DRealloc((void**) &p, size + CACHE_LINE)) != DD_OK)
                return error;

            new_offset = (size_t)(CACHE_LINE - ((ULONG_PTR)p & (CACHE_LINE - 1)));
            if (old_offset != new_offset)
                memmove(p + new_offset, p + old_offset, size);
            p += new_offset;
            ((size_t*)p)[-1] = new_offset;
        } else
            return MallocAligned(p_inout, size);
    } else if (size == 0) {
        FreeAligned(p);
        p = NULL;
    } else
        return DDERR_INVALIDPARAMS;
    *p_inout = p;
    return DD_OK;
}

//----------------------------------------------------------------------------
// Growing aligned buffer implementation.
//
HRESULT CAlignedBuffer32::Grow(DWORD growSize)
{
    if (allocatedBuf)
        D3DFree(allocatedBuf);
    size = growSize;
    if (D3DMalloc(&allocatedBuf, size + 31) != DD_OK)
    {
        allocatedBuf = 0;
        alignedBuf = 0;
        size = 0;
        return DDERR_OUTOFMEMORY;
    }
    alignedBuf = (LPVOID)(((ULONG_PTR)allocatedBuf + 31 ) & ~31);
    return D3D_OK;
}

#undef DPF_MODNAME
#define DPF_MODNAME "CBufferDDS::Grow"
//----------------------------------------------------------------------
// Growing buffer using DDS implementation.
//
HRESULT CBufferDDS::Grow(LPDIRECT3DDEVICEI lpDevI, DWORD growSize)
{
    DWORD dwRefCnt = 1;
    if (growSize <= size)
        return D3D_OK;
    if (allocatedBuf)
    {
        // Save reference count before deleting
        dwRefCnt = allocatedBuf->AddRef() - 1;
        // Release till gone!
        while (allocatedBuf->Release());
        allocatedBuf = NULL;
    }
    size = growSize;
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_CAPS;
    ddsd.dwWidth = size + 31;
    ddsd.ddsCaps.dwCaps = DDSCAPS_EXECUTEBUFFER | DDSCAPS_SYSTEMMEMORY;
    LPDIRECTDRAWSURFACE7 lpDDS7;
    HRESULT ret = lpDevI->lpDirect3DI->lpDD7->CreateSurface(&ddsd, &lpDDS7, NULL);
    if (ret != DD_OK)
    {
        D3D_ERR("Failed to allocate Vertex Buffer");
        size = 0;
        return ret;
    }
    ret = lpDDS7->QueryInterface(IID_IDirectDrawSurface, (LPVOID*)&allocatedBuf);
    if (ret != DD_OK)
    {
        D3D_ERR("failed to QI for DDS1");
        lpDDS7->Release();
        size = 0;
        return ret;
    }
    ret = lpDDS7->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
    if (ret != DD_OK)
    {
        D3D_ERR("Could not lock system memory Vertex Buffer.");
        lpDDS7->Release();
        allocatedBuf->Release();
        allocatedBuf = NULL;
        size = 0;
        return ret;
    }
    lpDDS7->Release();
    alignedBuf = ddsd.lpSurface;
    // Restore reference count
    while (--dwRefCnt)
        allocatedBuf->AddRef();
    return D3D_OK;
}
