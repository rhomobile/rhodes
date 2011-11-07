
#import <UIKit/UIKit.h>

#import "BarcodeViewFactory.h"
#import "BarcodeViewController.h"

#import <strings.h>


class BarcodeNativeView : public NativeView {
public:
	BarcodeNativeView() {
		//mView = nil;
	}
	virtual ~BarcodeNativeView() {
		//[mView release];
		//mView = nil;
	}
	
	virtual void* getView() {
		//if (mView == nil) {
		//	ctrl = []
		//	mView = [[RainbowView alloc] init];
		//}
		return [BarcodeViewController getCurrentView];
	}
	
	virtual void navigate(const char* url) {
		//[mView navigate:url];
	}
private:
	//MapViewControllerESRI* ctrl;
	//UIView* mView;
};



class BarcodeNativeViewFactory : public NativeViewFactory {
public:
	
	virtual NativeView* getNativeView(const char* viewType) {
		return new BarcodeNativeView();
	}
	
	virtual void destroyNativeView(NativeView* nativeView) {
		delete nativeView;
	}
	
};

static BarcodeNativeViewFactory* ourFactory = NULL; 

NativeViewFactory* BarcodeViewFactorySingletone::instance() {
	if (ourFactory == NULL) {
		ourFactory = new BarcodeNativeViewFactory();
	}
	return ourFactory;
}
