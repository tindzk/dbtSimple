#import <DateTime.h>

#import <Debit/HttpConnection.h>
#import <Debit/BufferResponse.h>

#define self resBusiness

class {
	/* You may modify this member variable but you must neither replace nor
	 * destroy it. Just make sure you're always using String_Copy() for
	 * overwriting its contents.
	 */
	String param;
	String param2;
	String param3;

	/* It makes sense to prefix locally created variables with a "_" to
	 * distinguish them from those initialised by Debit.
	 */
	String _local;
};

def(void, Init) {
	this->_local = String_Clone($("local variable"));
}

def(void, Destroy) {
	/* Debit doesn't know anything about this->_local. Therefore you need to
	 * free the memory on your own.
	 */
	String_Destroy(&this->_local);
}

action(ClientInformation) {
	String html = String_Format(
		$(
			"<b>Method:</b> %<br />"
			"<b>Referer:</b> \"%\"<br />"
			"<b>Session-ID:</b> \"%\"<br />"
			"<b>Contents of this->_local:</b> \"%\"<br />"
			"<b>The parameter 'param' contains:</b> \"%\"<br />"
			"<b>The parameter 'param2' contains:</b> \"%\"<br />"
			"<b>The parameter 'param3' contains:</b> \"%\"<br />"
			"Try <a href=\"/client/p1-p2-p3\">URL segments (1st route)</a>.<br />"
			"Try <a href=\"/client/URL-transmitted+value\">URL segments (2nd route)</a>.<br />"
			"Try a GET request:"
				"<form method=\"get\">"
					"<input type=\"text\" name=\"param\" />"
					"<input type=\"submit\" />"
				"</form>"
			"Try a POST request:"
				"<form method=\"post\">"
					"<input type=\"text\" name=\"param\" />"
					"<input type=\"submit\" />"
				"</form>"
		),

		HTTP_Method_ToString(req.method),
		req.referer.prot,
		req.sessionId.prot,
		this->_local.prot,
		this->param.prot,
		this->param2.prot,
		this->param3.prot);

	BufferResponse(resp, html);
}

action(Time) {
	String time = DateTime_Format(DateTime_GetCurrent());
	String msg = String_Format($("The server time is: %."), time.prot);
	String_Destroy(&time);

	/* The responsibility of freeing msg's memory is delegated to Debit which
	 * will destroy it as soon as it has been transmitted to the client.
	 */
	BufferResponse(resp, msg);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	/* `init' and `destroy' are optional. These are called automatically upon
	 * initialisation and destruction of the resource.
	 */
	.init    = (void *) ref(Init),
	.destroy = (void *) ref(Destroy),

	/* This shares the variables which are defined within the class. Debit will
	 * fill these automatically by their corresponding GET and POST values. You
	 * can also use them as a storage for the URL segments (see below).
	 *
	 * The final value is determined following this precedence order:
	 *   1. URL segments (see below)
	 *   2. GET parameters
	 *   3. POST parameters
	 */
	.members = {
		{ .name = $("param"),   Member(param)  },
		{ .name = $("param2"),  Member(param2) },
		{ .name = $("param3"),  Member(param3) }
	},

	.routes = {
		{ .path   = $("/client/{param}-{param2}-{param3}"),
		  .action = Action(ClientInformation) },

		{ .path   = $("/client/:param"),
		  .action = Action(ClientInformation) },

		{ .path   = $("/client"),
		  .action = Action(ClientInformation) },

		{ .path   = $("/time"),
		  .action = Action(Time) }
	}
};

RouterConstructor;
