;---------------------------Module-Header------------------------------;
; Module Name: glapi.inc
;
; OpenGL API function table indices.
;
; Created: 11/16/1993
; Author: Hock San Lee [hockl]
;
; Copyright (c) 1993 Microsoft Corporation
;----------------------------------------------------------------------;

; OpenGL function index in the dispatch table.
; NOTE: Extension indices are based on 336 non-extension indices

INDEX_glNewList                 equ     0
INDEX_glEndList                 equ     1
INDEX_glCallList                equ     2
INDEX_glCallLists               equ     3
INDEX_glDeleteLists             equ     4
INDEX_glGenLists                equ     5
INDEX_glListBase                equ     6
INDEX_glBegin                   equ     7
INDEX_glBitmap                  equ     8
INDEX_glColor3b                 equ     9
INDEX_glColor3bv                equ     10
INDEX_glColor3d                 equ     11
INDEX_glColor3dv                equ     12
INDEX_glColor3f                 equ     13
INDEX_glColor3fv                equ     14
INDEX_glColor3i                 equ     15
INDEX_glColor3iv                equ     16
INDEX_glColor3s                 equ     17
INDEX_glColor3sv                equ     18
INDEX_glColor3ub                equ     19
INDEX_glColor3ubv               equ     20
INDEX_glColor3ui                equ     21
INDEX_glColor3uiv               equ     22
INDEX_glColor3us                equ     23
INDEX_glColor3usv               equ     24
INDEX_glColor4b                 equ     25
INDEX_glColor4bv                equ     26
INDEX_glColor4d                 equ     27
INDEX_glColor4dv                equ     28
INDEX_glColor4f                 equ     29
INDEX_glColor4fv                equ     30
INDEX_glColor4i                 equ     31
INDEX_glColor4iv                equ     32
INDEX_glColor4s                 equ     33
INDEX_glColor4sv                equ     34
INDEX_glColor4ub                equ     35
INDEX_glColor4ubv               equ     36
INDEX_glColor4ui                equ     37
INDEX_glColor4uiv               equ     38
INDEX_glColor4us                equ     39
INDEX_glColor4usv               equ     40
INDEX_glEdgeFlag                equ     41
INDEX_glEdgeFlagv               equ     42
INDEX_glEnd                     equ     43
INDEX_glIndexd                  equ     44
INDEX_glIndexdv                 equ     45
INDEX_glIndexf                  equ     46
INDEX_glIndexfv                 equ     47
INDEX_glIndexi                  equ     48
INDEX_glIndexiv                 equ     49
INDEX_glIndexs                  equ     50
INDEX_glIndexsv                 equ     51
INDEX_glNormal3b                equ     52
INDEX_glNormal3bv               equ     53
INDEX_glNormal3d                equ     54
INDEX_glNormal3dv               equ     55
INDEX_glNormal3f                equ     56
INDEX_glNormal3fv               equ     57
INDEX_glNormal3i                equ     58
INDEX_glNormal3iv               equ     59
INDEX_glNormal3s                equ     60
INDEX_glNormal3sv               equ     61
INDEX_glRasterPos2d             equ     62
INDEX_glRasterPos2dv            equ     63
INDEX_glRasterPos2f             equ     64
INDEX_glRasterPos2fv            equ     65
INDEX_glRasterPos2i             equ     66
INDEX_glRasterPos2iv            equ     67
INDEX_glRasterPos2s             equ     68
INDEX_glRasterPos2sv            equ     69
INDEX_glRasterPos3d             equ     70
INDEX_glRasterPos3dv            equ     71
INDEX_glRasterPos3f             equ     72
INDEX_glRasterPos3fv            equ     73
INDEX_glRasterPos3i             equ     74
INDEX_glRasterPos3iv            equ     75
INDEX_glRasterPos3s             equ     76
INDEX_glRasterPos3sv            equ     77
INDEX_glRasterPos4d             equ     78
INDEX_glRasterPos4dv            equ     79
INDEX_glRasterPos4f             equ     80
INDEX_glRasterPos4fv            equ     81
INDEX_glRasterPos4i             equ     82
INDEX_glRasterPos4iv            equ     83
INDEX_glRasterPos4s             equ     84
INDEX_glRasterPos4sv            equ     85
INDEX_glRectd                   equ     86
INDEX_glRectdv                  equ     87
INDEX_glRectf                   equ     88
INDEX_glRectfv                  equ     89
INDEX_glRecti                   equ     90
INDEX_glRectiv                  equ     91
INDEX_glRects                   equ     92
INDEX_glRectsv                  equ     93
INDEX_glTexCoord1d              equ     94
INDEX_glTexCoord1dv             equ     95
INDEX_glTexCoord1f              equ     96
INDEX_glTexCoord1fv             equ     97
INDEX_glTexCoord1i              equ     98
INDEX_glTexCoord1iv             equ     99
INDEX_glTexCoord1s              equ     100
INDEX_glTexCoord1sv             equ     101
INDEX_glTexCoord2d              equ     102
INDEX_glTexCoord2dv             equ     103
INDEX_glTexCoord2f              equ     104
INDEX_glTexCoord2fv             equ     105
INDEX_glTexCoord2i              equ     106
INDEX_glTexCoord2iv             equ     107
INDEX_glTexCoord2s              equ     108
INDEX_glTexCoord2sv             equ     109
INDEX_glTexCoord3d              equ     110
INDEX_glTexCoord3dv             equ     111
INDEX_glTexCoord3f              equ     112
INDEX_glTexCoord3fv             equ     113
INDEX_glTexCoord3i              equ     114
INDEX_glTexCoord3iv             equ     115
INDEX_glTexCoord3s              equ     116
INDEX_glTexCoord3sv             equ     117
INDEX_glTexCoord4d              equ     118
INDEX_glTexCoord4dv             equ     119
INDEX_glTexCoord4f              equ     120
INDEX_glTexCoord4fv             equ     121
INDEX_glTexCoord4i              equ     122
INDEX_glTexCoord4iv             equ     123
INDEX_glTexCoord4s              equ     124
INDEX_glTexCoord4sv             equ     125
INDEX_glVertex2d                equ     126
INDEX_glVertex2dv               equ     127
INDEX_glVertex2f                equ     128
INDEX_glVertex2fv               equ     129
INDEX_glVertex2i                equ     130
INDEX_glVertex2iv               equ     131
INDEX_glVertex2s                equ     132
INDEX_glVertex2sv               equ     133
INDEX_glVertex3d                equ     134
INDEX_glVertex3dv               equ     135
INDEX_glVertex3f                equ     136
INDEX_glVertex3fv               equ     137
INDEX_glVertex3i                equ     138
INDEX_glVertex3iv               equ     139
INDEX_glVertex3s                equ     140
INDEX_glVertex3sv               equ     141
INDEX_glVertex4d                equ     142
INDEX_glVertex4dv               equ     143
INDEX_glVertex4f                equ     144
INDEX_glVertex4fv               equ     145
INDEX_glVertex4i                equ     146
INDEX_glVertex4iv               equ     147
INDEX_glVertex4s                equ     148
INDEX_glVertex4sv               equ     149
INDEX_glClipPlane               equ     150
INDEX_glColorMaterial           equ     151
INDEX_glCullFace                equ     152
INDEX_glFogf                    equ     153
INDEX_glFogfv                   equ     154
INDEX_glFogi                    equ     155
INDEX_glFogiv                   equ     156
INDEX_glFrontFace               equ     157
INDEX_glHint                    equ     158
INDEX_glLightf                  equ     159
INDEX_glLightfv                 equ     160
INDEX_glLighti                  equ     161
INDEX_glLightiv                 equ     162
INDEX_glLightModelf             equ     163
INDEX_glLightModelfv            equ     164
INDEX_glLightModeli             equ     165
INDEX_glLightModeliv            equ     166
INDEX_glLineStipple             equ     167
INDEX_glLineWidth               equ     168
INDEX_glMaterialf               equ     169
INDEX_glMaterialfv              equ     170
INDEX_glMateriali               equ     171
INDEX_glMaterialiv              equ     172
INDEX_glPointSize               equ     173
INDEX_glPolygonMode             equ     174
INDEX_glPolygonStipple          equ     175
INDEX_glScissor                 equ     176
INDEX_glShadeModel              equ     177
INDEX_glTexParameterf           equ     178
INDEX_glTexParameterfv          equ     179
INDEX_glTexParameteri           equ     180
INDEX_glTexParameteriv          equ     181
INDEX_glTexImage1D              equ     182
INDEX_glTexImage2D              equ     183
INDEX_glTexEnvf                 equ     184
INDEX_glTexEnvfv                equ     185
INDEX_glTexEnvi                 equ     186
INDEX_glTexEnviv                equ     187
INDEX_glTexGend                 equ     188
INDEX_glTexGendv                equ     189
INDEX_glTexGenf                 equ     190
INDEX_glTexGenfv                equ     191
INDEX_glTexGeni                 equ     192
INDEX_glTexGeniv                equ     193
INDEX_glFeedbackBuffer          equ     194
INDEX_glSelectBuffer            equ     195
INDEX_glRenderMode              equ     196
INDEX_glInitNames               equ     197
INDEX_glLoadName                equ     198
INDEX_glPassThrough             equ     199
INDEX_glPopName                 equ     200
INDEX_glPushName                equ     201
INDEX_glDrawBuffer              equ     202
INDEX_glClear                   equ     203
INDEX_glClearAccum              equ     204
INDEX_glClearIndex              equ     205
INDEX_glClearColor              equ     206
INDEX_glClearStencil            equ     207
INDEX_glClearDepth              equ     208
INDEX_glStencilMask             equ     209
INDEX_glColorMask               equ     210
INDEX_glDepthMask               equ     211
INDEX_glIndexMask               equ     212
INDEX_glAccum                   equ     213
INDEX_glDisable                 equ     214
INDEX_glEnable                  equ     215
INDEX_glFinish                  equ     216
INDEX_glFlush                   equ     217
INDEX_glPopAttrib               equ     218
INDEX_glPushAttrib              equ     219
INDEX_glMap1d                   equ     220
INDEX_glMap1f                   equ     221
INDEX_glMap2d                   equ     222
INDEX_glMap2f                   equ     223
INDEX_glMapGrid1d               equ     224
INDEX_glMapGrid1f               equ     225
INDEX_glMapGrid2d               equ     226
INDEX_glMapGrid2f               equ     227
INDEX_glEvalCoord1d             equ     228
INDEX_glEvalCoord1dv            equ     229
INDEX_glEvalCoord1f             equ     230
INDEX_glEvalCoord1fv            equ     231
INDEX_glEvalCoord2d             equ     232
INDEX_glEvalCoord2dv            equ     233
INDEX_glEvalCoord2f             equ     234
INDEX_glEvalCoord2fv            equ     235
INDEX_glEvalMesh1               equ     236
INDEX_glEvalPoint1              equ     237
INDEX_glEvalMesh2               equ     238
INDEX_glEvalPoint2              equ     239
INDEX_glAlphaFunc               equ     240
INDEX_glBlendFunc               equ     241
INDEX_glLogicOp                 equ     242
INDEX_glStencilFunc             equ     243
INDEX_glStencilOp               equ     244
INDEX_glDepthFunc               equ     245
INDEX_glPixelZoom               equ     246
INDEX_glPixelTransferf          equ     247
INDEX_glPixelTransferi          equ     248
INDEX_glPixelStoref             equ     249
INDEX_glPixelStorei             equ     250
INDEX_glPixelMapfv              equ     251
INDEX_glPixelMapuiv             equ     252
INDEX_glPixelMapusv             equ     253
INDEX_glReadBuffer              equ     254
INDEX_glCopyPixels              equ     255
INDEX_glReadPixels              equ     256
INDEX_glDrawPixels              equ     257
INDEX_glGetBooleanv             equ     258
INDEX_glGetClipPlane            equ     259
INDEX_glGetDoublev              equ     260
INDEX_glGetError                equ     261
INDEX_glGetFloatv               equ     262
INDEX_glGetIntegerv             equ     263
INDEX_glGetLightfv              equ     264
INDEX_glGetLightiv              equ     265
INDEX_glGetMapdv                equ     266
INDEX_glGetMapfv                equ     267
INDEX_glGetMapiv                equ     268
INDEX_glGetMaterialfv           equ     269
INDEX_glGetMaterialiv           equ     270
INDEX_glGetPixelMapfv           equ     271
INDEX_glGetPixelMapuiv          equ     272
INDEX_glGetPixelMapusv          equ     273
INDEX_glGetPolygonStipple       equ     274
INDEX_glGetString               equ     275
INDEX_glGetTexEnvfv             equ     276
INDEX_glGetTexEnviv             equ     277
INDEX_glGetTexGendv             equ     278
INDEX_glGetTexGenfv             equ     279
INDEX_glGetTexGeniv             equ     280
INDEX_glGetTexImage             equ     281
INDEX_glGetTexParameterfv       equ     282
INDEX_glGetTexParameteriv       equ     283
INDEX_glGetTexLevelParameterfv  equ     284
INDEX_glGetTexLevelParameteriv  equ     285
INDEX_glIsEnabled               equ     286
INDEX_glIsList                  equ     287
INDEX_glDepthRange              equ     288
INDEX_glFrustum                 equ     289
INDEX_glLoadIdentity            equ     290
INDEX_glLoadMatrixf             equ     291
INDEX_glLoadMatrixd             equ     292
INDEX_glMatrixMode              equ     293
INDEX_glMultMatrixf             equ     294
INDEX_glMultMatrixd             equ     295
INDEX_glOrtho                   equ     296
INDEX_glPopMatrix               equ     297
INDEX_glPushMatrix              equ     298
INDEX_glRotated                 equ     299
INDEX_glRotatef                 equ     300
INDEX_glScaled                  equ     301
INDEX_glScalef                  equ     302
INDEX_glTranslated              equ     303
INDEX_glTranslatef              equ     304
INDEX_glViewport                equ     305
INDEX_glArrayElement            equ     306
INDEX_glBindTexture             equ     307
INDEX_glColorPointer            equ     308
INDEX_glDisableClientState      equ     309
INDEX_glDrawArrays              equ     310
INDEX_glDrawElements            equ     311
INDEX_glEdgeFlagPointer         equ     312
INDEX_glEnableClientState       equ     313
INDEX_glIndexPointer            equ     314
INDEX_glIndexub                 equ     315
INDEX_glIndexubv                equ     316
INDEX_glInterleavedArrays       equ     317
INDEX_glNormalPointer           equ     318
INDEX_glPolygonOffset           equ     319
INDEX_glTexCoordPointer         equ     320
INDEX_glVertexPointer           equ     321
INDEX_glAreTexturesResident     equ     322
INDEX_glCopyTexImage1D          equ     323
INDEX_glCopyTexImage2D          equ     324
INDEX_glCopyTexSubImage1D       equ     325
INDEX_glCopyTexSubImage2D       equ     326
INDEX_glDeleteTextures          equ     327
INDEX_glGenTextures             equ     328
INDEX_glGetPointerv             equ     329
INDEX_glIsTexture               equ     330
INDEX_glPrioritizeTextures      equ     331
INDEX_glTexSubImage1D           equ     332
INDEX_glTexSubImage2D           equ     333
INDEX_glPopClientAttrib         equ     334
INDEX_glPushClientAttrib        equ     335
INDEX_glLastFunc                equ     335

INDEX_glDrawRangeElementsWIN    equ     (INDEX_glLastFunc+1)
INDEX_glColorTableEXT           equ     (INDEX_glLastFunc+2)
INDEX_glColorSubTableEXT        equ     (INDEX_glLastFunc+3)
INDEX_glGetColorTableEXT        equ     (INDEX_glLastFunc+4)
INDEX_glGetColorTableParameterivEXT equ (INDEX_glLastFunc+5)
INDEX_glGetColorTableParameterfvEXT equ (INDEX_glLastFunc+6)
INDEX_glCurrentTextureIndexWIN	equ     (INDEX_glLastFunc+7)
INDEX_glMultiTexCoord1dWIN	equ     (INDEX_glLastFunc+8)
INDEX_glMultiTexCoord1dvWIN	equ     (INDEX_glLastFunc+9)
INDEX_glMultiTexCoord1fWIN	equ     (INDEX_glLastFunc+10)
INDEX_glMultiTexCoord1fvWIN	equ     (INDEX_glLastFunc+11)
INDEX_glMultiTexCoord1iWIN	equ     (INDEX_glLastFunc+12)
INDEX_glMultiTexCoord1ivWIN	equ     (INDEX_glLastFunc+13)
INDEX_glMultiTexCoord1sWIN	equ     (INDEX_glLastFunc+14)
INDEX_glMultiTexCoord1svWIN	equ     (INDEX_glLastFunc+15)
INDEX_glMultiTexCoord2dWIN	equ     (INDEX_glLastFunc+16)
INDEX_glMultiTexCoord2dvWIN	equ     (INDEX_glLastFunc+17)
INDEX_glMultiTexCoord2fWIN	equ     (INDEX_glLastFunc+18)
INDEX_glMultiTexCoord2fvWIN	equ     (INDEX_glLastFunc+19)
INDEX_glMultiTexCoord2iWIN	equ     (INDEX_glLastFunc+20)
INDEX_glMultiTexCoord2ivWIN	equ     (INDEX_glLastFunc+21)
INDEX_glMultiTexCoord2sWIN	equ     (INDEX_glLastFunc+22)
INDEX_glMultiTexCoord2svWIN	equ     (INDEX_glLastFunc+23)
INDEX_glMultiTexCoord3dWIN	equ     (INDEX_glLastFunc+24)
INDEX_glMultiTexCoord3dvWIN	equ     (INDEX_glLastFunc+25)
INDEX_glMultiTexCoord3fWIN	equ     (INDEX_glLastFunc+26)
INDEX_glMultiTexCoord3fvWIN	equ     (INDEX_glLastFunc+27)
INDEX_glMultiTexCoord3iWIN	equ     (INDEX_glLastFunc+28)
INDEX_glMultiTexCoord3ivWIN	equ     (INDEX_glLastFunc+29)
INDEX_glMultiTexCoord3sWIN	equ     (INDEX_glLastFunc+30)
INDEX_glMultiTexCoord3svWIN	equ     (INDEX_glLastFunc+31)
INDEX_glMultiTexCoord4dWIN	equ     (INDEX_glLastFunc+32)
INDEX_glMultiTexCoord4dvWIN	equ     (INDEX_glLastFunc+33)
INDEX_glMultiTexCoord4fWIN	equ     (INDEX_glLastFunc+34)
INDEX_glMultiTexCoord4fvWIN	equ     (INDEX_glLastFunc+35)
INDEX_glMultiTexCoord4iWIN	equ     (INDEX_glLastFunc+36)
INDEX_glMultiTexCoord4ivWIN	equ     (INDEX_glLastFunc+37)
INDEX_glMultiTexCoord4sWIN	equ     (INDEX_glLastFunc+38)
INDEX_glMultiTexCoord4svWIN	equ     (INDEX_glLastFunc+39)
INDEX_glBindNthTextureWIN	equ     (INDEX_glLastFunc+40)
INDEX_glNthTexCombineFuncWIN	equ     (INDEX_glLastFunc+41)

; OpenGL function index in the dispatch table cache in the TEB.

FASTINDEX_glCallList            equ     0
FASTINDEX_glCallLists           equ     1
FASTINDEX_glBegin               equ     2
FASTINDEX_glColor3b             equ     3
FASTINDEX_glColor3bv            equ     4
FASTINDEX_glColor3d             equ     5
FASTINDEX_glColor3dv            equ     6
FASTINDEX_glColor3f             equ     7
FASTINDEX_glColor3fv            equ     8
FASTINDEX_glColor3i             equ     9
FASTINDEX_glColor3iv            equ     10
FASTINDEX_glColor3s             equ     11
FASTINDEX_glColor3sv            equ     12
FASTINDEX_glColor3ub            equ     13
FASTINDEX_glColor3ubv           equ     14
FASTINDEX_glColor3ui            equ     15
FASTINDEX_glColor3uiv           equ     16
FASTINDEX_glColor3us            equ     17
FASTINDEX_glColor3usv           equ     18
FASTINDEX_glColor4b             equ     19
FASTINDEX_glColor4bv            equ     20
FASTINDEX_glColor4d             equ     21
FASTINDEX_glColor4dv            equ     22
FASTINDEX_glColor4f             equ     23
FASTINDEX_glColor4fv            equ     24
FASTINDEX_glColor4i             equ     25
FASTINDEX_glColor4iv            equ     26
FASTINDEX_glColor4s             equ     27
FASTINDEX_glColor4sv            equ     28
FASTINDEX_glColor4ub            equ     29
FASTINDEX_glColor4ubv           equ     30
FASTINDEX_glColor4ui            equ     31
FASTINDEX_glColor4uiv           equ     32
FASTINDEX_glColor4us            equ     33
FASTINDEX_glColor4usv           equ     34
FASTINDEX_glEdgeFlag            equ     35
FASTINDEX_glEdgeFlagv           equ     36
FASTINDEX_glEnd                 equ     37
FASTINDEX_glIndexd              equ     38
FASTINDEX_glIndexdv             equ     39
FASTINDEX_glIndexf              equ     40
FASTINDEX_glIndexfv             equ     41
FASTINDEX_glIndexi              equ     42
FASTINDEX_glIndexiv             equ     43
FASTINDEX_glIndexs              equ     44
FASTINDEX_glIndexsv             equ     45
FASTINDEX_glNormal3b            equ     46
FASTINDEX_glNormal3bv           equ     47
FASTINDEX_glNormal3d            equ     48
FASTINDEX_glNormal3dv           equ     49
FASTINDEX_glNormal3f            equ     50
FASTINDEX_glNormal3fv           equ     51
FASTINDEX_glNormal3i            equ     52
FASTINDEX_glNormal3iv           equ     53
FASTINDEX_glNormal3s            equ     54
FASTINDEX_glNormal3sv           equ     55
FASTINDEX_glTexCoord1d          equ     56
FASTINDEX_glTexCoord1dv         equ     57
FASTINDEX_glTexCoord1f          equ     58
FASTINDEX_glTexCoord1fv         equ     59
FASTINDEX_glTexCoord1i          equ     60
FASTINDEX_glTexCoord1iv         equ     61
FASTINDEX_glTexCoord1s          equ     62
FASTINDEX_glTexCoord1sv         equ     63
FASTINDEX_glTexCoord2d          equ     64
FASTINDEX_glTexCoord2dv         equ     65
FASTINDEX_glTexCoord2f          equ     66
FASTINDEX_glTexCoord2fv         equ     67
FASTINDEX_glTexCoord2i          equ     68
FASTINDEX_glTexCoord2iv         equ     69
FASTINDEX_glTexCoord2s          equ     70
FASTINDEX_glTexCoord2sv         equ     71
FASTINDEX_glTexCoord3d          equ     72
FASTINDEX_glTexCoord3dv         equ     73
FASTINDEX_glTexCoord3f          equ     74
FASTINDEX_glTexCoord3fv         equ     75
FASTINDEX_glTexCoord3i          equ     76
FASTINDEX_glTexCoord3iv         equ     77
FASTINDEX_glTexCoord3s          equ     78
FASTINDEX_glTexCoord3sv         equ     79
FASTINDEX_glTexCoord4d          equ     80
FASTINDEX_glTexCoord4dv         equ     81
FASTINDEX_glTexCoord4f          equ     82
FASTINDEX_glTexCoord4fv         equ     83
FASTINDEX_glTexCoord4i          equ     84
FASTINDEX_glTexCoord4iv         equ     85
FASTINDEX_glTexCoord4s          equ     86
FASTINDEX_glTexCoord4sv         equ     87
FASTINDEX_glVertex2d            equ     88
FASTINDEX_glVertex2dv           equ     89
FASTINDEX_glVertex2f            equ     90
FASTINDEX_glVertex2fv           equ     91
FASTINDEX_glVertex2i            equ     92
FASTINDEX_glVertex2iv           equ     93
FASTINDEX_glVertex2s            equ     94
FASTINDEX_glVertex2sv           equ     95
FASTINDEX_glVertex3d            equ     96
FASTINDEX_glVertex3dv           equ     97
FASTINDEX_glVertex3f            equ     98
FASTINDEX_glVertex3fv           equ     99
FASTINDEX_glVertex3i            equ     100
FASTINDEX_glVertex3iv           equ     101
FASTINDEX_glVertex3s            equ     102
FASTINDEX_glVertex3sv           equ     103
FASTINDEX_glVertex4d            equ     104
FASTINDEX_glVertex4dv           equ     105
FASTINDEX_glVertex4f            equ     106
FASTINDEX_glVertex4fv           equ     107
FASTINDEX_glVertex4i            equ     108
FASTINDEX_glVertex4iv           equ     109
FASTINDEX_glVertex4s            equ     110
FASTINDEX_glVertex4sv           equ     111
FASTINDEX_glMaterialf           equ     112
FASTINDEX_glMaterialfv          equ     113
FASTINDEX_glMateriali           equ     114
FASTINDEX_glMaterialiv          equ     115
FASTINDEX_glDisable             equ     116
FASTINDEX_glEnable              equ     117
FASTINDEX_glPopAttrib           equ     118
FASTINDEX_glPushAttrib          equ     119
FASTINDEX_glEvalCoord1d         equ     120
FASTINDEX_glEvalCoord1dv        equ     121
FASTINDEX_glEvalCoord1f         equ     122
FASTINDEX_glEvalCoord1fv        equ     123
FASTINDEX_glEvalCoord2d         equ     124
FASTINDEX_glEvalCoord2dv        equ     125
FASTINDEX_glEvalCoord2f         equ     126
FASTINDEX_glEvalCoord2fv        equ     127
FASTINDEX_glEvalPoint1          equ     128
FASTINDEX_glEvalPoint2          equ     129
FASTINDEX_glLoadIdentity        equ     130
FASTINDEX_glLoadMatrixf         equ     131
FASTINDEX_glLoadMatrixd         equ     132
FASTINDEX_glMatrixMode          equ     133
FASTINDEX_glMultMatrixf         equ     134
FASTINDEX_glMultMatrixd         equ     135
FASTINDEX_glPopMatrix           equ     136
FASTINDEX_glPushMatrix          equ     137
FASTINDEX_glRotated             equ     138
FASTINDEX_glRotatef             equ     139
FASTINDEX_glScaled              equ     140
FASTINDEX_glScalef              equ     141
FASTINDEX_glTranslated          equ     142
FASTINDEX_glTranslatef          equ     143
FASTINDEX_glArrayElement        equ     144
FASTINDEX_glBindTexture         equ     145
FASTINDEX_glColorPointer        equ     146
FASTINDEX_glDisableClientState  equ     147
FASTINDEX_glDrawArrays          equ     148
FASTINDEX_glDrawElements        equ     149
FASTINDEX_glEdgeFlagPointer     equ     150
FASTINDEX_glEnableClientState   equ     151
FASTINDEX_glIndexPointer        equ     152
FASTINDEX_glIndexub             equ     153
FASTINDEX_glIndexubv            equ     154
FASTINDEX_glInterleavedArrays   equ     155
FASTINDEX_glNormalPointer       equ     156
FASTINDEX_glPolygonOffset       equ     157
FASTINDEX_glTexCoordPointer     equ     158
FASTINDEX_glVertexPointer       equ     159
FASTINDEX_glGetPointerv         equ     160
FASTINDEX_glPopClientAttrib     equ     161
FASTINDEX_glPushClientAttrib    equ     162
FASTINDEX_glDrawRangeElementsWIN equ    163
FASTINDEX_glColorTableEXT       equ     164
FASTINDEX_glColorSubTableEXT    equ     165
FASTINDEX_glCurrentTextureIndexWIN equ  166
FASTINDEX_glBindNthTextureWIN	equ     167
FASTINDEX_glNthTexCombineFuncWIN equ    168
FASTINDEX_glMultiTexCoord1fWIN	equ     169
FASTINDEX_glMultiTexCoord1fvWIN	equ     170
FASTINDEX_glMultiTexCoord1iWIN	equ     171
FASTINDEX_glMultiTexCoord1ivWIN	equ     172
FASTINDEX_glMultiTexCoord2fWIN	equ     173
FASTINDEX_glMultiTexCoord2fvWIN	equ     174
FASTINDEX_glMultiTexCoord2iWIN	equ     175
FASTINDEX_glMultiTexCoord2ivWIN	equ     176