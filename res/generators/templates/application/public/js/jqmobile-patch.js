(function($){

    function fixFormsAction() {
        $('form[data-action-fix!="false"][data-action-fix!="no"]').each(function(idx, form){
            $(form).attr('method', 'GET');
        });
    }

    $(document).ready(function(){
        // if forms action needs to be fixed for WP7 or BB6 platforms then..
        if ("true" == $('body').attr('data-do-fix-forms')) {
            // do fix actions for the forms already loaded
            fixFormsAction();
            // and for any future forms loaded with AJAX
            $('body').ajaxSuccess(function(){
                fixFormsAction();
            });
        }
    });

    // Rho: Android 2.2 doesn't define WebKitTransitionEvent, though animations
    // still work
    if (navigator.userAgent.indexOf("Android 2") != -1) {
        //$.support.WebKitAnimationEvent = true;
        $.support.cssTransitions = true;
    }

    // Set to 30 seconds; Ajax request doesn't come back if >= 60
    $.ajaxSetup({
        timeout: 30000,
        headers: {
            'Transition-Enabled': true
        }
    });

    $.ajaxPrefilter(function(options, originalOptions, jqXHR){
        // we may have no explicit success handler!
        if (!options.success) return;

        var origSuccess = options.success;
        options.success = function(html, textStatus, jqXHR) {
            if (jqXHR.getResponseHeader('Wait-Page')) {
                // We cannot just do nothing on wait-page being received, because
                // at this moment jQM already have isPageTransitioning lock is set.
                // Due to this lock is private part of jQM we have no control on it.
                // So we are going to tag Wait-Page HTML content by some HTML attribute
                // to detect it in "pagebeforechange" event handler and then perform
                // preventDefault() to let jQM to release isPageTransitioning lock.
                //origSuccess('<div data-role="page" data-rho-wait-page="true"><!-- intentionally empty --></div>');
                origSuccess(html.replace(/data-role=["']page["']/,
                    'data-role="page" data-rho-wait-page="true"'));
            } else {
                origSuccess(html);
            }
        }

    });

/*
    $(document).bind( "pagechange", function(e, data) {
        // We only want to handle changePage() calls where the caller is
        // providing us an already loaded page.
        if ( !(typeof data.toPage === "string") ) {
            var pageDiv = data.toPage[0];
            if ("true" === pageDiv.getAttribute("data-rho-wait-page")) {
                if (0 < $.mobile.urlHistory.activeIndex) {
                    $.mobile.urlHistory.activeIndex--;
                    $.mobile.urlHistory.clearForward();
                } else {
                    $.mobile.urlHistory.stack = [];
                }
            }
        }
    });

    $(document).bind( "pagebeforechange", function(e, data) {
        // We only want to handle changePage() calls where the caller is
        // providing us an already loaded page.
        if ( !(typeof data.toPage === "string") ) {
            var pageDiv = data.toPage[0];
            if ("true" === pageDiv.getAttribute("data-rho-wait-page")) {
                //Make sure to tell changePage() we've handled this call so it doesn't
                //have to do anything. So jQM can release isPageTransitioning lock
                e.preventDefault();
            }
        }
    });
*/

    //shared page enhancements
	function enhancePage( $page, role ) {
		// If a role was specified, make sure the data-role attribute
		// on the page element is in sync.
		if( role ) {
			$page.attr( "data-" + $.mobile.ns + "role", role );
		}

		//run page plugin
		$page.page();
	}

    // hijack $.mobile.loadPage function
    var path = $.mobile.path;
    var original_loadPage = $.mobile.loadPage;
    // introduce custom initialization parameter support
    original_loadPage.defaults.loadMsgDelay = $.mobile.loadingMessageDelay || original_loadPage.defaults.loadMsgDelay;

    $.mobile.loadPage = function( url, options ) {

        // if no page HTML code provided, then call original function
        if(!options["html"]) {
            return original_loadPage(url, options);
        }

		// This function uses deferred notifications to let callers
		// know when the page is done loading, or if an error has occurred.
		var deferred = $.Deferred(),

			// The default loadPage options with overrides specified by
			// the caller.
			settings = $.extend( {}, $.mobile.loadPage.defaults, options ),

			// The DOM element for the page after it has been loaded.
			page = null,

			// If the reloadPage option is true, and the page is already
			// in the DOM, dupCachedPage will be set to the page element
			// so that it can be removed after the new version of the
			// page is loaded off the network.
			dupCachedPage = null,

			// determine the current base url
			findBaseWithDefault = function(){
				var closestBase = ( $.mobile.activePage && getClosestBaseUrl( $.mobile.activePage ) );
				return closestBase || documentBase.hrefNoHash;
			},

            /*
			// The absolute version of the URL passed into the function. This
			// version of the URL may contain dialog/subpage params in it.
			absUrl = path.makeUrlAbsolute( url, findBaseWithDefault() );
            */
            absUrl = "";

		// If the caller provided data, and we're using "get" request,
		// append the data to the URL.
		if ( settings.data && settings.type === "get" ) {
			absUrl = path.addSearchParams( absUrl, settings.data );
			settings.data = undefined;
		}

		// If the caller is using a "post" request, reloadPage must be true
		if(  settings.data && settings.type === "post" ){
			settings.reloadPage = true;
		}

			// The absolute version of the URL minus any dialog/subpage params.
			// In otherwords the real URL of the page to be loaded.
		var fileUrl = path.getFilePath( absUrl ),

			// The version of the Url actually stored in the data-url attribute of
			// the page. For embedded pages, it is just the id of the page. For pages
			// within the same domain as the document base, it is the site relative
			// path. For cross-domain pages (Phone Gap only) the entire absolute Url
			// used to load the page.
			dataUrl = path.convertUrlToDataUrl( absUrl );

		// Make sure we have a pageContainer to work with.
		settings.pageContainer = settings.pageContainer || $.mobile.pageContainer;

		// Check to see if the page already exists in the DOM.
		page = settings.pageContainer.children( ":jqmData(url='" + dataUrl + "')" );

		// If we failed to find the page, check to see if the url is a
		// reference to an embedded page. If so, it may have been dynamically
		// injected by a developer, in which case it would be lacking a data-url
		// attribute and in need of enhancement.
		if ( page.length === 0 && dataUrl && !path.isPath( dataUrl ) ) {
			page = settings.pageContainer.children( "#" + dataUrl )
				.attr( "data-" + $.mobile.ns + "url", dataUrl );
		}

		// If we failed to find a page in the DOM, check the URL to see if it
		// refers to the first page in the application. If it isn't a reference
		// to the first page and refers to non-existent embedded page, error out.
        if ( page.length === 0 ) {
            if ( $.mobile.firstPage && fileUrl && path.isFirstPageUrl( fileUrl ) ) {
                // Check to make sure our cached-first-page is actually
                // in the DOM. Some user deployed apps are pruning the first
                // page from the DOM for various reasons, we check for this
                // case here because we don't want a first-page with an id
                // falling through to the non-existent embedded page error
                // case. If the first-page is not in the DOM, then we let
                // things fall through to the ajax loading code below so
                // that it gets reloaded.
                if ( $.mobile.firstPage.parent().length ) {
                    page = $( $.mobile.firstPage );
                }
            } else if ( fileUrl && path.isEmbeddedPage( fileUrl )  ) {
                deferred.reject( absUrl, options );
                return deferred.promise();
            }
        }

        /*
		// Reset base to the default document base.
		if ( base ) {
			base.reset();
		}
        */

        /*
		// If the page we are interested in is already in the DOM,
		// and the caller did not indicate that we should force a
		// reload of the file, we are done. Otherwise, track the
		// existing page as a duplicated.
		if ( page.length ) {
			if ( !settings.reloadPage ) {
				enhancePage( page, settings.role );
				deferred.resolve( absUrl, options, page );
				return deferred.promise();
			}
			dupCachedPage = page;
		}
        */

        var mpc = settings.pageContainer,
            pblEvent = new $.Event( "pagebeforeload" ),
            triggerData = { url: url, absUrl: absUrl, dataUrl: dataUrl, deferred: deferred, options: settings };

        // Let listeners know we're about to load a page.
        mpc.trigger( pblEvent, triggerData );

        // If the default behavior is prevented, stop here!
        if( pblEvent.isDefaultPrevented() ){
            return deferred.promise();
        }

        if ( settings.showLoadMsg ) {

            // This configurable timeout allows cached pages a brief delay to load without showing a message
            var loadMsgDelay = setTimeout(function(){
                    $.mobile.showPageLoadingMsg();
                }, settings.loadMsgDelay ),

                // Shared logic for clearing timeout and removing message.
                hideMsg = function(){

                    // Stop message show timer
                    clearTimeout( loadMsgDelay );

                    // Hide loading message
                    $.mobile.hidePageLoadingMsg();
                };
        }

        setHtml(options.html);

        function setHtml( html ) {
            //pre-parse html to check for a data-url,
            //use it as the new fileUrl, base path, etc
            var all = $( "<div></div>" ),

                    //page title regexp
                    newPageTitle = html.match( /<title[^>]*>([^<]*)/ ) && RegExp.$1,

                    // TODO handle dialogs again
                    pageElemRegex = new RegExp( "(<[^>]+\\bdata-" + $.mobile.ns + "role=[\"']?page[\"']?[^>]*>)" ),
                    dataUrlRegex = new RegExp( "\\bdata-" + $.mobile.ns + "url=[\"']?([^\"'>]*)[\"']?" );


            // data-url must be provided for the base tag so resource requests can be directed to the
            // correct url. loading into a temprorary element makes these requests immediately
            if( pageElemRegex.test( html )
                    && RegExp.$1
                    && dataUrlRegex.test( RegExp.$1 )
                    && RegExp.$1 ) {
                url = fileUrl = path.getFilePath( RegExp.$1 );
            }
            /*
            else{

            }
            */

            /*
            if ( base ) {
                base.set( fileUrl );
            }
            */

            //workaround to allow scripts to execute when included in page divs
            all.get( 0 ).innerHTML = html;
            page = all.find( ":jqmData(role='page'), :jqmData(role='dialog')" ).first();

            //if page elem couldn't be found, create one and insert the body element's contents
            if( !page.length ){
                page = $( "<div data-" + $.mobile.ns + "role='page'>" + html.split( /<\/?body[^>]*>/gmi )[1] + "</div>" );
            }

            if ( newPageTitle && !page.jqmData( "title" ) ) {
                if ( ~newPageTitle.indexOf( "&" ) ) {
                    newPageTitle = $( "<div>" + newPageTitle + "</div>" ).text();
                }
                page.jqmData( "title", newPageTitle );
            }

            //rewrite src and href attrs to use a base url
            if( !$.support.dynamicBaseTag ) {
                var newPath = path.get( fileUrl );
                page.find( "[src], link[href], a[rel='external'], :jqmData(ajax='false'), a[target]" ).each(function() {
                    var thisAttr = $( this ).is( '[href]' ) ? 'href' :
                            $(this).is('[src]') ? 'src' : 'action',
                        thisUrl = $( this ).attr( thisAttr );

                    // XXX_jblas: We need to fix this so that it removes the document
                    //            base URL, and then prepends with the new page URL.
                    //if full path exists and is same, chop it - helps IE out
                    thisUrl = thisUrl.replace( location.protocol + '//' + location.host + location.pathname, '' );

                    if( !/^(\w+:|#|\/)/.test( thisUrl ) ) {
                        $( this ).attr( thisAttr, newPath + thisUrl );
                    }
                });
            }

            //append to page and enhance
            // TODO taging a page with external to make sure that embedded pages aren't removed
            //      by the various page handling code is bad. Having page handling code in many
            //      places is bad. Solutions post 1.0
            page
                .attr( "data-" + $.mobile.ns + "url", path.convertUrlToDataUrl( fileUrl ) )
                .attr( "data-" + $.mobile.ns + "external-page", true )
                .appendTo( settings.pageContainer );

            // wait for page creation to leverage options defined on widget
            page.one( 'pagecreate', $.mobile._bindPageRemove );

            enhancePage( page, settings.role );

            // Enhancing the page may result in new dialogs/sub pages being inserted
            // into the DOM. If the original absUrl refers to a sub-page, that is the
            // real page we are interested in.
            if ( absUrl.indexOf( "&" + $.mobile.subPageUrlKey ) > -1 ) {
                page = settings.pageContainer.children( ":jqmData(url='" + dataUrl + "')" );
            }

            //bind pageHide to removePage after it's hidden, if the page options specify to do so

            // Remove loading message.
            if ( settings.showLoadMsg ) {
                hideMsg();
            }

            // Add the page reference to our triggerData.
            //triggerData.xhr = xhr;
            //triggerData.textStatus = textStatus;
            triggerData.page = page;

            // Let listeners know the page loaded successfully.
            settings.pageContainer.trigger( "pageload", triggerData );

            deferred.resolve( absUrl, options, page, dupCachedPage );
        }

		return deferred.promise();
	};
    // copy original defaults
    $.mobile.loadPage.defaults = original_loadPage.defaults;

    function insertAsyncPage(data) {
        $.mobile.loadPage("inline://", {html: data})
            .done(function( url, options, newPage, dupCachedPage ) {
                options.duplicateCachedPage = dupCachedPage;
                $.mobile.changePage( newPage, options );
            });
    }

    // It looks like fireOnce:true option of endlessScroll
    // works quite unstable (so far it is version 1.4.8),
    // so using lock variable here.
    // Without this lock callback fired really twice each
    // time on Google Nexus One. Also it fired twice from
    // time to time on other browsers.
    var endlessScrollInProgress = false;

    // initialized pages (not instances!)
    var endlessScrollPages = {};

    // trigger flag for endless scroll reset
    var doResetEndlessScroll = false;

    // This function set scroll callback for active page instance.
    // It doesn't set endless scroll event handler for whole document.
    // usage sample:
    //      setupEndlessScroll( "div.productPage", "ul.productList",
    //                          "li.loadingMessage", "/app/Product/getpage?page=");
    //
    // scrollOptions is optional parameter, look at plugin documentation for details
    function setupEndlessScroll(pageSelector, listSelector, loadingMsgItemSelector, pageBaseUrl, scrollOptions) {
        // Do nothing if jquery.endless-scroll.js plugin is not available
        if(!$.fn.endlessScroll) return;

        // don't setup pageinit evebt handler twice for the same page!
        if (endlessScrollPages[pageSelector]) return;
        endlessScrollPages[pageSelector] = pageSelector;  // just to have debug info, any value will do indeed

        $(pageSelector).live('pageshow', function(event) {
            // trigger scroll pages counter reset
            doResetEndlessScroll = true;

            var list = $.mobile.activePage.find(listSelector);
            $.mobile.activePage.endlessScrollCallback = function(i, scrollDone) {
                // store list at very first call to optimize DOM searches number
                //list = list || $.mobile.activePage.find(listSelector);

                // it scrolls page top every time, so unable to be used
                //$.mobile.showPageLoadingMsg('loading..');
                // ---
                // Instead, we turn "loading" message on in the last item
                // look for definition in the /public/css/application.css
                list.addClass("loading");

                $.get(pageBaseUrl + i, function(data) {
                    var loadingMsgItem = list.find(loadingMsgItemSelector);
                    loadingMsgItem.before(data);
                    list.listview("refresh");

                    // turn "loading" message off in the last item
                    list.removeClass("loading");
                    //$.mobile.hidePageLoadingMsg();
                    scrollDone();
                });
            };
        });
    }

    // setup endless scroll plugin and
    // common handler for a whole document
    $(document).ready(function(){
        if($.fn.endlessScroll) {
            $(document).endlessScroll({
                // let it be, but no much hope so far
                fireOnce: true,
                bottomPixels: 150,
                resetCounter: function() {
                    var doReset = doResetEndlessScroll;
                    doResetEndlessScroll = false;
                    return doReset;
                },
                callback: function(i) {
                    // our own "fireOnce" feature
                    if (endlessScrollInProgress) return;
                    endlessScrollInProgress = true;

                    if ($.mobile.activePage.endlessScrollCallback) {
                        $.mobile.activePage.endlessScrollCallback(i, function(){ endlessScrollInProgress = false; });
                    } else {
                        endlessScrollInProgress = false;
                    }
                    return false;
                }
            });
        }
    });

    window.Rho = window.Rho || {};
    window.Rho.jqm = $.mobile;
    // Rho: add insertAsyncPage callback for transitions
    window.Rho.insertAsyncPage = insertAsyncPage;
    window.Rho.setupEndlessScroll = setupEndlessScroll;

})(jQuery);
