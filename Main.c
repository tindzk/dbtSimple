#import <Main.h>
#import <Server.h>
#import <String.h>
#import <Logger.h>
#import <Signal.h>
#import <Integer.h>
#import <Terminal.h>
#import <Exception.h>
#import <GenericClientListener.h>

#import <Debit/HttpConnection.h>
#import <Debit/SessionManager.h>

Logger logger;
Terminal term;

void OnLogMessage(__unused void *ptr, FmtString msg, Logger_Level level, ProtString file, int line) {
	ProtString slevel = Logger_ResolveLevel(level);
	String sline = Integer_ToString(line);

	Terminal_FmtPrint(&term,
		$("[%] $ (%:%)\n"),
		slevel, msg, file, sline.prot);

	String_Destroy(&sline);
}

bool startServer(Server *server, ClientListener listener) {
	try {
		Server_Init(server, 8080, listener);
		Logger_Info(&logger, $("Server started."));
		excReturn true;
	} catch(Socket, AddressInUse) {
		Logger_Error(&logger, $("The address is already in use!"));
		excReturn false;
	} finally {

	} tryEnd;

	return false;
}

bool Main (
	__unused ProtString base,
	__unused ProtStringArray *args,
	__unused ProtStringArray *env
) {
	term = Terminal_New(File_StdIn, File_StdOut, true);

	Logger_Init(&logger, Callback(NULL, OnLogMessage),
		Logger_Level_Fatal |
		Logger_Level_Crit  |
		Logger_Level_Error |
		Logger_Level_Warn  |
		Logger_Level_Info  |
		Logger_Level_Debug |
		Logger_Level_Trace);

	GenericClientListener listener;
	GenericClientListener_Init(&listener, HttpConnection_GetImpl());

	Server server;

	if (!startServer(&server,
		GenericClientListener_AsClientListener(&listener)))
	{
		return false;
	}

	try {
		while (true) {
			Server_Process(&server);
		}
	} catch(Signal, SigInt) {
		Logger_Info(&logger, $("Server shutdown."));
	} finally {
		Server_Destroy(&server);
		Terminal_Destroy(&term);
	} tryEnd;

	return true;
}
