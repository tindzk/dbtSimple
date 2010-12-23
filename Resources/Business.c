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
};

action(ClientInformation) {
	String html = String_Format(
		$(
			"<b>Method:</b> %<br />"
			"<b>Referer:</b> \"%\"<br />"
			"<b>Session-ID:</b> \"%\"<br />"
			"<b>The parameter 'param' contains:</b> \"%\"<br />"
			"Try <a href=\"/client/URL-transmitted+value\">URL segments</a>."
			"<br />Try a GET request:"
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
		req.referer,
		req.sessionId,
		this->param);

	BufferResponse(resp, html);
}

action(Time) {
	String time = DateTime_Format(DateTime_GetCurrent());
	String msg = String_Format($("The server time is: %."), time);
	String_Destroy(&time);

	/* The responsibility of freeing msg's memory is delegated to Debit which
	 * will destroy it as soon as it has been transmitted to the client.
	 */
	BufferResponse(resp, msg);
}

ImplEx(Resource) = {
	.size = sizeof(self),

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
		{ .name = $("param"),  Member(param) },
	},

	.routes = {
		{ .path   = $("/client/$param"),
		  .action = Action(ClientInformation) },

		{ .path   = $("/client"),
		  .action = Action(ClientInformation) },

		{ .path   = $("/time"),
		  .action = Action(Time) }
	}
};

RouterConstructor;
