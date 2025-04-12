git submodule update --init --remote
wget -O ThirdParty/Niflect/file1.zip "https://drive.usercontent.google.com/download?id=182fv2wlo1AKxTOH37VosCu18GICwdiHQ&export=download&authuser=0&confirm=t&uuid=b7230614-6a97-4c7a-afd0-418cd83886d3&at=APcmpoxON3sAUnH57WDvaIvUQ7Bx:1744472449906"
wget -O ThirdParty/NiflectGenTool/file2.zip "https://drive.usercontent.google.com/download?id=1Q2WGtpVsj1nuqklss_NIG6U9jp2mB77U&export=download&authuser=0&confirm=t&uuid=dbb3a3a0-9b36-4a54-b4ef-33a587544caf&at=APcmpowi6Oc-i64KCatJec-N3E0f:1744472463946"
unzip ThirdParty/Niflect/file1.zip -d ThirdParty/Niflect
unzip ThirdParty/NiflectGenTool/file2.zip -d ThirdParty/NiflectGenTool
rm ThirdParty/Niflect/file1.zip
rm ThirdParty/NiflectGenTool/file2.zip