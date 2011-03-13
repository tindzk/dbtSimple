#import <Debit/Application.h>

#define self Application

/* These two hooks are optional. It is even safe to delete the two functions. */

def(void, OnInit) {
	Logger_Info(&this->logger, $("Initialization hook called."));
}

def(void, OnDestroy) {
	Logger_Info(&this->logger, $("Destruction hook called."));
}
