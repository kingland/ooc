@echo OFF
REM
REM Running test cases as a test suite
REM 
REM Windows version - without memory leak
REM
REM #########################################x


@ooctest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

@exceptiontest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

REM @testcasetest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

@refcountedtest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

@vectortest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

@listtest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

@signaltest
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

EXIT /B 0

:TESTS_FAILED

EXIT /B 1