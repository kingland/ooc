@echo OFF
REM
REM Complete check for the WIN32 version of the ooc library
REM 
REM Steps:
REM		1.	Build a complete ooc library:
REM				- Start MS Visual Studio
REM				- open the ooc solution
REM				- From the menu: Build -> Batch Build -> select all libooc configurations
REM				- Press Build or Rebuild
REM		2.	In the Unit directory start this batch file
REM			This batch file runs all testcases against all versions of libooc.
REM
REM ######################################################################################

@echo Running all testcases against all versions of libooc.
@echo You did not forget a complete rebuild of libooc, did you?
@echo.

==============================================================

@nmake /f Makefile.Windows clean 
@nmake /f Makefile.Windows CONFIG=Debug

IF NOT %ERRORLEVEL%==0 GOTO BUILD_FAILED

call suite
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

==============================================================

@nmake /f Makefile.Windows clean
@nmake /f Makefile.Windows CONFIG=Debug_st

IF NOT %ERRORLEVEL%==0 GOTO BUILD_FAILED

call suite
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

==============================================================

@nmake /f Makefile.Windows clean
@nmake /f Makefile.Windows CONFIG=Release

IF NOT %ERRORLEVEL%==0 GOTO BUILD_FAILED

call suite
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

==============================================================

@nmake /f Makefile.Windows clean
@nmake /f Makefile.Windows CONFIG=Release_st

IF NOT %ERRORLEVEL%==0 GOTO BUILD_FAILED

call suite
IF NOT %ERRORLEVEL%==0 GOTO TESTS_FAILED

==============================================================

@nmake /f Makefile.Windows clean

@echo.
@echo All tests done! Everything is ok.

EXIT /B 0

==============================================================

:TESTS_FAILED
@echo.
@echo #########################################################
@echo #                                                       #
@echo #             T E S T S    F A I L E D                  #
@echo #                                                       #
@echo #      Running testcases failed, process aborted.       #
@echo #                                                       #
@echo #########################################################

EXIT /B 1

==============================================================

:BUILD_FAILED
@echo.
@echo #########################################################
@echo #                                                       #
@echo #             B U I L D    F A I L E D                  #
@echo #                                                       #
@echo #      Building testcases failed, process aborted.      #
@echo #                                                       #
@echo #########################################################

EXIT /B 1
