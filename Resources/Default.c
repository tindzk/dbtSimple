#import <Debit/HttpConnection.h>
#import <Debit/BufferResponse.h>
#import <Debit/RedirectResponse.h>

#define self resDefault

/* A class must be available for each resource. It can be kept empty though. */
class {

};

action(Home) {
	BufferResponse(resp, $$(
		"Click <a href=\"/non-existent-route\">here</a> to test the fall-back route.<br />"
		"Click <a href=\"/client\">here</a> for your browser information.<br />"
		"Click <a href=\"/time\">here</a> for the server time.<br />"
	));
}

action(Redirection) {
	RedirectResponse(resp, $$("/time"));
}

ImplEx(Resource) = {
	.name = $("Default"),
	.size = sizeof(self),

	.members = {

	},

	.routes = {
		/* Each route must be connected to an action. */
		{ .path   = $("/"),
		  .action = Action(Home) },

		/* You can define an optional fall-back route which is used by default
		 * when nothing else matches.
		 */
		{ .path   = $("*"),
		  .action = Action(Redirection) }
	}
};

RouterConstructor;
