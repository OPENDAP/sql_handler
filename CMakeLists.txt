
# sql_handler CMakeLists.txt file for use with CLion. This is not intended to be
# a replacement for the autotols build. But it does provide information that
# CLion needs to navigate the Hyrax code. jhrg 12/5/19

# Included because we want the IDE to know about the library sources, not
# the installed headers. This might make for some consistency issues if the
# libdap code is edited and not installed. jhrg 12/5/19
include_directories(../../../libdap4)

# Does including this _and_ the above bork CLion? jhrg 1.15.20
include_directories(../../../build/include/libdap)

include_directories(/usr/local/Cellar/openssl/1.0.2q/include/)
include_directories(/usr/local/Cellar/openssl/1.0.2r/include/)
include_directories(/usr/local/include/)

# Where the hyrax project puts the dependencies' headers

include_directories(../../../build/deps/include)


include_directories(../../ppt)
include_directories(../../ppt/unit-tests)
include_directories(../../server)
include_directories(../../server/test)
include_directories(../../standalone)
include_directories(../../templates)
include_directories(../../xmlcommand)
include_directories(../../xmlcommand/not_used)
include_directories(../../xmlcommand/tests)
include_directories(../../xmlcommand/unit-tests)
include_directories(../../xmlcommand/unused)

add_executable(sql_handler.so
    action/SQLAction.h
    action/SQLActionFactory.h
    action/SQLActionList.h
    action/SQLActionManager.h
    action/SQLDynamicActionFactory.h
    action/SQLDynamicActionList.h
    action/SQLObjectAction.h
    action/SQLObjectActionFactory.h
    action/SQLObjectActionList.h
    action/SQLStaticAction.h
    action/SQLStaticActionList.h
    connector/SQLConnector.h
    connector/SQLErrorConnector.h
    connector/SQLHandleConnector.h
    connector/SQLSimpleConnector.h
    connector/SQLTypeConnector.h
    container/SQLAttribute.h
    container/SQLConstraint.h
    container/SQLContainer.cc
    container/SQLContainer.h
    container/SQLContainerDefinitions.h
    container/SQLContainerFactory.cc
    container/SQLContainerFactory.h
    container/SQLContainerStorage.h
    container/SQLContainerStorageVolatile.cc
    container/SQLContainerStorageVolatile.h
    container/SQLDataHandlerInterface.h
    container/SQLQuery.cc
    container/SQLQuery.h
    container/SQLQueryDefinitions.h
    container/SQLTextContainer.cc
    container/SQLTextContainer.h
    container/SQLTextDefinitions.h
    DEM/SQLCheckPoint.cc
    DEM/SQLCheckPoint.h
    DEM/SQLDefaultErrorAction.h
    DEM/SQLErrorFactory.h
    DEM/SQLErrorManager.h
    DEM/SQLInternalError.h
    DEM/SQLInternalFatalError.h
    DTM/SQLBaseType.h
    DTM/SQLCastAction.h
    DTM/SQLComplexType.h
    DTM/SQLDummySimpleType.h
    DTM/SQLDynamicTypeFactory.h
    DTM/SQLObjectTypeFactory.h
    DTM/SQLSequence.h
    DTM/SQLSimpleType.h
    DTM/SQLTypeFactoryComponent.h
    DTM/SQLTypeManager.h
    handler/connector/SQLCloseAction.h
    handler/connector/SQLConnectAction.h
    handler/connector/SQLNextTypeAction.h
    handler/connector/SQLQueryAction.h
    handler/SQLBuildDAS.h
    handler/SQLBuildDATA.h
    handler/SQLBuildDDS.h
    templates/Connector.cc
    templates/Connector.h
    templates/ErrorFactoryComponent.cc
    templates/ErrorFactoryComponent.h
    templates/Module.cc
    templates/Module.h
    templates/Plugin.cc
    templates/Plugin.h
    templates/PluginLoader.cc
    templates/PluginLoader.h
    templates/ResponseNames.h
    templates/TypeFactoryComponent.cc
    templates/TypeFactoryComponent.h
    templates/Types.h
    unixODBC/ODBCConnector.cc
    unixODBC/ODBCConnector.h
    unixODBC/ODBCErrorFactoryComponent.cc
    unixODBC/ODBCErrorFactoryComponent.h
    unixODBC/ODBCPlugin.cc
    unixODBC/ODBCPlugin.h
    unixODBC/ODBCPluginLoader.cc
    unixODBC/ODBCPluginLoader.h
    unixODBC/ODBCTypeFactoryComponent.cc
    unixODBC/ODBCTypeFactoryComponent.h
    unixODBC/ODBCTypes.h
    utils/Clone.h
    utils/Delete.h
    utils/SmartCast.h
    utils/SmartCheck.h
    utils/SmartList.h
    utils/SmartMap.h
    utils/StringMatch.cc
    utils/StringMatch.h
    config.h
    sql_config.h
    SQLDefinitions.h
    SQLLinker.h
    SQLModule.cc
    SQLModule.h
    SQLPlugin.cc
    SQLPlugin.h
    SQLPluginList.h
    SQLRequestHandler.cc
    SQLRequestHandler.h
    SQLResponseNames.h
)
