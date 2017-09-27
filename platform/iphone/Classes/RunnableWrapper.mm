/*------------------------------------------------------------------------
 * (The MIT License)
 *
 * Copyright (c) 2014-2014 Rhomobile, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://rhomobile.com
 *------------------------------------------------------------------------*/

#import "RunnableWrapper.h"
#import "Rhodes.h"

@interface RunnableWrapper : NSObject
+ (void)run:(NSValue*)task;
@end

@implementation RunnableWrapper
+ (void)run:(NSValue*)task {
    rho::common::IRhoRunnable* tsk = static_cast<rho::common::IRhoRunnable*>([task pointerValue]);
    if ((tsk != NULL) /*&& (tsk != [NSNull null])*/) {
        tsk->runObject();
    }
}
@end

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask) {
    if (pTask != NULL) {
        [Rhodes performOnUiThread:[RunnableWrapper class] arg:[NSValue valueWithPointer:pTask] wait:NO];
    }
}