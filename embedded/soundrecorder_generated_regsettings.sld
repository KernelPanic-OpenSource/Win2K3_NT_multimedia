<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "Mantis.DTD">

  <DCARRIER
    CarrierRevision="1"
    DTDRevision="16"
  >
    <TASKS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </TASKS>

    <PLATFORMS
      Context="1"
    >    </PLATFORMS>

    <REPOSITORIES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </REPOSITORIES>

    <GROUPS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </GROUPS>

    <COMPONENTS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >
      <COMPONENT
        ComponentVSGUID="{9B657D2B-491A-403B-89B6-F771FE770C28}"
        ComponentVIGUID="{1E5A72F6-F7A9-407D-AE34-0BE24DAEA135}"
        Revision="1"
        RepositoryVSGUID="{00000000-0000-0000-0000-000000000000}"
        Visibility="1000"
        MultiInstance="False"
        Released="False"
        Editable="True"
        HTMLFinal="False"
        IsMacro="False"
        Opaque="False"
        Context="1"
        PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
      >
        <PROPERTIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <PROPERTY
            Name="cmiConcordanceID"
            Format="String"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >SoundRecorder_Generated_Regsettings.inf:DefaultInstall</PROPERTY>
        </PROPERTIES>

        <RESOURCES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\.wav</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >SoundRec</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav\OpenWithList\sndrec32.exe&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1001"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\.wav\OpenWithList\sndrec32.exe</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav\OpenWithList\sndrec32.exe&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\.wav\OpenWithList\sndrec32.exe&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\AppID\{00020C01-0000-0000-C000-000000000046}&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1002"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\AppID\{00020C01-0000-0000-C000-000000000046}</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >Sound Recorder</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\AppID\{00020C01-0000-0000-C000-000000000046}&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\AppID\{00020C01-0000-0000-C000-000000000046}&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1003"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >Sound</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Insertable&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1004"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Insertable</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Insertable&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Insertable&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Ole1Class&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1005"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Ole1Class</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >SoundRec</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Ole1Class&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\Ole1Class&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\ProgID&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1006"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\ProgID</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >SoundRec</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\ProgID&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\ProgID&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\TreatAs&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1007"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\TreatAs</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{00020C01-0000-0000-C000-000000000046}</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\TreatAs&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\CLSID\{0003000D-0000-0000-C000-000000000046}\TreatAs&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1008"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >Wave Sound</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\CLSID&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1009"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\CLSID</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{00020C01-0000-0000-C000-000000000046}</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\CLSID&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\CLSID&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\DefaultIcon&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1010"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\DefaultIcon</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >sndrec32.exe,1</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\DefaultIcon&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\DefaultIcon&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1011"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1012"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute\server&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1013"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute\server</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >sndrec32.exe</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute\server&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdExecute\server&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1014"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\server&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1015"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\server</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >sndrec32.exe</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\server&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\server&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1016"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\0&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1017"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\0</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >&amp;Play</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\0&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\0&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\1&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1018"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >&amp;Edit</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\1&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\1&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\2&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1019"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\2</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >&amp;Open</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\2&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\protocol\StdFileEditing\verb\2&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1020"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\shell\record</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >&amp;Record</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record\command&quot;,&quot;&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1021"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
            >
              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >{9B657D2B-491A-403B-89B6-F771FE770C28}</PROPERTY>

              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >HKEY_CLASSES_ROOT\SoundRec\shell\record\command</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              >sndrec32.exe &quot;%L&quot;</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record\command&quot;,&quot;&quot;</DISPLAYNAME>

            <DESCRIPTION>RegKey(819):&quot;HKEY_CLASSES_ROOT\SoundRec\shell\record\command&quot;,&quot;&quot;</DESCRIPTION>
          </RESOURCE>
        </RESOURCES>

        <DEPENDENCIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >        </DEPENDENCIES>

        <DISPLAYNAME>SoundRecorder_Generated_Regsettings.inf</DISPLAYNAME>

        <VERSION>Version 1.0</VERSION>

        <DESCRIPTION>SoundRecorder_Generated_Regsettings.inf</DESCRIPTION>

        <VENDOR></VENDOR>

        <OWNERS>jayraja</OWNERS>

        <AUTHORS>jayraja</AUTHORS>

        <DATECREATED>8/7/2001</DATECREATED>

        <DATEREVISED>8/7/2001</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
