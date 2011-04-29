#import <Timer.h>
#import <Debit/HttpConnection.h>
#import <Debit/BufferResponse.h>

#define self resTimer

class {
	size_t cnt;
	Response *resp;
	RdString seconds;
};

def(bool, OnTimer, __unused u64 dropped) {
	BufferResponse(this->resp, $$("Timer completed."));
	return false;
}

action(Wait) {
	this->resp = resp;

	try {
		u8 seconds = UInt8_Parse(this->seconds);

		if (seconds == 0) {
			excGoto error;
		}

		Timer timer = Timer_New(ClockType_Monotonic);
		Timer_SetTimer(&timer, seconds);

		Tasks_Enqueue(tasks,
			Timer_AsTask(&timer, Timer_OnTimer_For(this, ref(OnTimer))));
	} catchModule(Integer) {
		excGoto error;
	} finally {

	} tryEnd;

	when (error) {
		BufferResponse(resp, $$("Invalid time specified."));
	}
}

def(bool, OnInterval, __unused u64 dropped) {
	if (this->cnt == 3) {
		BufferResponse(this->resp, $$("Interval triggered three times."));

		/* Detach timer. */
		return false;
	}

	this->cnt++;

	return true;
}

action(Interval) {
	this->cnt  = 1;
	this->resp = resp;

	try {
		u8 seconds = UInt8_Parse(this->seconds);

		if (seconds == 0) {
			excGoto error;
		}

		Timer timer = Timer_New(ClockType_Monotonic);
		Timer_SetInterval(&timer, seconds);

		Tasks_Enqueue(tasks,
			Timer_AsTask(&timer, Timer_OnTimer_For(this, ref(OnInterval))));
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
