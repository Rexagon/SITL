:: Запускаем сборку библиотеки
call build.bat || exit /B 1

:: Запускаем создание установочника
iscc.exe /q ./setup/build_setup.iss || exit /B 1
