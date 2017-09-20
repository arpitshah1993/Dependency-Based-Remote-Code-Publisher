start .\Debug\Server.exe "8082" "Server/WebPages/" "Server/Repository/" "*.cpp" "*.h"
start .\Debug\TestExecutiveClient.exe "8081" "8082"
start .\Client\bin\Debug\Client.exe
