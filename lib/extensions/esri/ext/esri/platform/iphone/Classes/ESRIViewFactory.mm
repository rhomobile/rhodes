
#import <UIKit/UIKit.h>
#import "MapViewControllerESRI.h"

#import "ESRIViewFactory.h"

#import <strings.h>


class ESRINativeView : public NativeView {
public:
	ESRINativeView() {
		//mView = nil;
	}
	virtual ~ESRINativeView() {
		//[mView release];
		//mView = nil;
	}
	
	virtual void* getView() {
		//if (mView == nil) {
		//	ctrl = []
		//	mView = [[RainbowView alloc] init];
		//}
		return [MapViewControllerESRI getCurrentView];
	}
	
	virtual void navigate(const char* url) {
		//[mView navigate:url];
	}
private:
	//MapViewControllerESRI* ctrl;
	//UIView* mView;
};



class ESRINativeViewFactory : public NativeViewFactory {
public:
	
	virtual NativeView* getNativeView(const char* viewType) {
		return new ESRINativeView();
	}
	
	virtual void destroyNativeView(NativeView* nativeView) {
		delete nativeView;
	}
	
};

static ESRINativeViewFactory* ourFactory = NULL; 

NativeViewFactory* ESRIViewFactorySingletone::instance() {
	if (ourFactory == NULL) {
		ourFactory = new ESRINativeViewFactory();
	}
	return ourFactory;
}
