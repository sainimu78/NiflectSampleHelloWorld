set(v_IntegratedToolName NiflectGenTool)
set(v_IntegratedToolRootDirPath ${c_RootThirdPartyDirPath}/${v_IntegratedToolName})
set(v_UnzippedDirPath ${v_IntegratedToolRootDirPath}/${v_IntegratedToolName})
set(v_ZipFileName ${v_IntegratedToolName}.zip)
set(v_SrcAddrZipFilePath ${c_StorageAddrPath}/Release/${c_ProjectPlatform}/${v_ZipFileName})

set(v_NiflectGenToolBinDirPath ${v_UnzippedDirPath}/build/${c_ProjectPlatform}/${c_ProjectArch}/Release/${c_BinDirName})

include(${c_RootCMakeProjectFrameworkDirPath}/IntegrateNiflectGenTool.cmake)