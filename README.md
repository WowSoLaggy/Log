# Log v1.2

Log library is the one-header-only library that provides the log of any text to the console and to the given file simultaneously. Thread-safety is guaranteed (or at least should be).
There is an option to rotate the log file if it exceeds the given size on the log init.
Also the VersionRetriever class is provided to acquire product version from file (used for the welcome logger message).

Class usage is briefly described in the Log.h file. Templates are used only to store static members, never mind.

Changelog:

v1.2
- log rotation is now supported (only on log init)