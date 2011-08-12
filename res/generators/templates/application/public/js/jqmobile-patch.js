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
        var origSuccess = options.success;

        options.success = function(html, textStatus, jqXHR) {
            if (jqXHR.getResponseHeader('Wait-Page')) {
                // do nothing
            } else {
                origSuccess(html);
            }
        }

    });

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

    var path = $.mobile.path;
    var original_loadPage = $.mobile.loadPage;

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
                    pageElemRegex = new RegExp( ".*(<[^>]+\\bdata-" + $.mobile.ns + "role=[\"']?page[\"']?[^>]*>).*" ),
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
            page
                .attr( "data-" + $.mobile.ns + "url", path.convertUrlToDataUrl( fileUrl ) )
                .appendTo( settings.pageContainer );

            // wait for page creation to leverage options defined on widget
            page.one('pagecreate', function(){

                // when dom caching is not enabled bind to remove the page on hide
                if( !page.data("page").options.domCache ){
                    page.bind( "pagehide.remove", function(){
                        $(this).remove();
                    });
                }
            });

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

            deferred.resolve( absUrl, options, page, dupCachedPage );
        }

		return deferred.promise();
	};

    function insertAsyncPage(data) {
        setTimeout(function(){
            /*$('.waiting').remove();*/
            $.mobile.hidePageLoadingMsg();
        },450);

        $.mobile.loadPage("inline://", {html: data})
            .done(function( url, options, newPage, dupCachedPage ) {
                options.duplicateCachedPage = dupCachedPage;
                $.mobile.changePage( newPage, options );
            });
    }

    window.Rho = window.Rho || {};
    window.Rho.jqm = $.mobile;
    // Rho: add insertAsyncPage callback for transitions
    window.Rho.insertAsyncPage = insertAsyncPage;

})(jQuery);
