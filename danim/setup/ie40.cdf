[Version]
Class=IEXPRESS
CDFVersion=3

[Options]
PackagePurpose=InstallApp
ShowInstallProgramWindow=1
HideExtractAnimation=1
UseLongFileName=1
InsideCompressed=0
CAB_FixedSize=0
CAB_ResvCodeSigning=6144
RebootMode=N
InstallPrompt=%InstallPrompt%
DisplayLicense=%DisplayLicense%
FinishMessage=%FinishMessage%
TargetName=%TargetName%
FriendlyName=%FriendlyName%
AppLaunched=%AppLaunched%
PostInstallCmd=%PostInstallCmd%
AdminQuietInstCmd=%AdminQuietInstCmd%
UserQuietInstCmd=%UserQuietInstCmd%
SourceFiles=SourceFiles

[Strings]
InstallPrompt=
DisplayLicense=
FinishMessage=
TargetName=d:\appel\setup\retail.bin\axaie4.exe
FriendlyName=DirectX Media Installation
AppLaunched=axaie4.inf
PostInstallCmd=<None>
AdminQuietInstCmd=
UserQuietInstCmd=
FILE0="danim.dll"
FILE2="dxmedia.zip"
FILE4="unaxaie4.inf"
FILE5="axaie4.inf"

[SourceFiles]
SourceFiles0=D:\appel\setup\retail.bin\
SourceFiles2=D:\appel\build\win\ship\bin\
SourceFiles3=D:\appel\setup\
[SourceFiles0]
%FILE0%=
[SourceFiles2]
%FILE2%=
[SourceFiles3]
%FILE4%=
%FILE5%=
