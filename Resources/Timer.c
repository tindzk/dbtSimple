#import <EventLoop.h>
#import <Debit/HttpConnection.h>
#import <Debit/BufferResponse.h>

#define self resTimer

class {
	size_t cnt;
	Response *resp;
	RdString seconds;
};

def(void, OnTimer, __unused u64 dropped, __unused Timer *timer) {
	BufferResponse(this->resp, $$("Timer completed."));
}

action(Wait) {
	this->resp = resp;

	try {
		u8 seconds = UInt8_Parse(this->seconds);

		if (seconds == 0) {
			excGoto error;
		}

		EventLoop_AddTimer(EventLoop_GetInstance(),
			seconds, EventLoop_OnTimer_For(this, ref(OnTimer)));
	} catchModule(Integer) {
		excGoto error;
	} finally {

	} tryEnd;

	when (error) {
		BufferResponse(resp, $$("Invalid time specified."));
	}
}

def(void, OnInterval, __unused u64 dropped, Timer *timer) {
	if (this->cnt == 3) {
		EventLoop_DetachTimer(EventLoop_GetInstance(), timer);
		BufferResponse(this->resp, $$("Interval triggered three times."));

		/* After BufferResponse() and Response_Flush(), respectively, accessing
		 * member variables is forbidden as the resource has been most likely
		 * already destroyed.
		 */
		return;
	}

	this->cnt++;
}

action(Interval) {
	this->cnt  = 1;
	this->resp = resp;

	try {
		u8 seconds = UInt8_Parse(this->seconds);

		if (seconds == 0) {
			excGoto error;
		}

		EventLoop_AddIntervalTimer(EventLoop_GetInstance(),
			seconds, EventLoop_OnTimer_For(this, ref(OnInterval)));
	} catchModule(Integer) {
		excGoto error;
	} finally {

	} tryEnd;

	when (error) {
		BufferResponse(resp, $$("Invalid time specified."));
	}
}

ImplEx(Resource) = {
	.name = $("Timer"),
	.size = sizeof(self),

	.members = {
		{ .name = $("seconds"), Member(seconds) }
	},

	.routes = {
		{ .path   = $("/timer/wait/:seconds"),
		  .action = ref(Wait) },

		{ .path   = $("/timer/interval/:seconds"),
		  .action = ref(Interval) }
	}
};

RouterConstructor;
