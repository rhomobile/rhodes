(function($) {

    function publicInterface() {
        return {
            actions: actions,
            SyncNotify: SyncNotify,
            SyncNotification: SyncNotification,
            byEvent: notifyByEvent
        };
    }

    var rho = RhoSync.rho;

    var actions = {
        'none': 0,
        'delete': 1,
        'update': 2,
        'create': 3
    };

    function SyncNotification(params, removeAfterFire){
        this.params = params || '';
        this.removeAfterFire = removeAfterFire || false;
        this.toString = function() {
            return "SyncNotification({removeAfterFire: " +this.removeAfterFire +"})";
        }
    }

    function SyncNotify(engine) {

        var LOG = new rho.Logger('SyncNotify');

        var srcIDAndObject = {};
        var singleObjectSrcName = '';
        var singleObjectID = '';
        var hashCreateObjectErrors = {};
        var searchNotification = null;
        var syncNotifications = {};
        var allNotification = null;
        var emptyNotify = SyncNotification();
        var /*ISyncStatusListener*/ syncStatusListener = null;
        var enableReporting = false;
        var enableReportingGlobal = true;
        var strNotifyBody = "";
        var hashSrcObjectCount = {};


        function addObjectNotify(source, objectId) {
            if ("string" == typeof source) { // if source by name
                singleObjectSrcName = source;
                singleObjectID = objectId.match(/^\{/) ? objectId.substring(1, objectId.length-2) : objectId ;
            } else { // else it is source by id or by reference
                var srcId = ("number" == typeof source) ? source : /*then it is an object*/ source.id;
                if (srcId) {
                    var hashObject = srcIDAndObject[srcId];
                    if (hashObject) {
                        hashObject = {};
                        srcIDAndObject[srcId] = hashObject;
                    }
                    hashObject[objectId] = actions.none;
                }
            }
        }

        function cleanObjectNotifications() {
            singleObjectSrcName = "";
            singleObjectID = "";
            srcIDAndObject = {};
        }

        this.cleanCreateObjectErrors = function() {
            hashCreateObjectErrors = {};
        };

        function processSingleObject() {
            if (!singleObjectSrcName) return;

            var src = engine.sources[singleObjectSrcName];
            if (src) {
                addObjectNotify(src,singleObjectID);
            }
            singleObjectSrcName = "";
            singleObjectID = "";
        }

        this.fireObjectsNotification = function() {
            var body = {};
            var strBody = "";

            $.each(srcIDAndObject, function(srcId, hashObject) {
                $.each(hashObject, function(strObject, nNotifyType) {

                    if (nNotifyType == actions.none) return;

                    if (strBody) {
                        strBody += "&rho_callback=1&";
                    }

                    if (nNotifyType == actions['delete']) {
                        strBody += "deleted[][object]=" + strObject;
                        strBody += "&deleted[][source_id]=" + srcId;
                    } else if (nNotifyType == actions.update) {
                        strBody += "updated[][object]=" + strObject;
                        strBody += "&updated[][source_id]=" + srcId;
                    } else if (nNotifyType == actions.create) {
                        strBody += "created[][object]=" + strObject;
                        strBody += "&created[][source_id]=" + srcId;
                    }

                    hashObject[strObject] = actions.none;
                });
            });

            if (!strBody) return;
            callNotify(new SyncNotification("", false), strBody);
        };

        this.onObjectChanged = function(srcId, objectId, actionType) {
            processSingleObject();

            var hashObject = srcIDAndObject[srcId];
            if (!hashObject) return;

            if(objectId in hashObject) {
                hashObject[objectId] = actionType;
            }
        };

        function addCreateObjectError(srcId, objectId, error) {
            var hashErrors = hashCreateObjectErrors.get(srcId);
            if ( hashErrors == null ) {
                hashErrors = {};
                hashCreateObjectErrors[srcId] = hashErrors;
            }
            hashErrors[objectId] = error;
        }

        function makeCreateObjectErrorBody(srcId) {
            var hashErrors = hashCreateObjectErrors[srcId];
            if (!hashErrors) return "";

            var strBody = "";
            $.each(srcIDAndObject, function(strObject, strError) {
                strBody += "&create_error[][object]=" + strObject;
                strBody += "&create_error[][error_message]=" + strError;
            });
            return strBody;
        }

         this.onSyncSourceEnd = function(nSrc, sourcesArray) {
            var src = sourcesArray[nSrc];

            if (engine.getState() == engine.states.stop && src.errCode != rho.errors.ERR_NONE) {
                var pSN = getSyncNotifyBySrc(src);
                if (pSN != null) {
                    this.fireSyncNotification(src, true, src.errCode, "");
                } else {
                    this.fireAllSyncNotifications(true, src.errCode, src.error, "");
                }
            }
            else {
                this.fireSyncNotification(src, true, src.errCode, "");
            }

            this.cleanCreateObjectErrors();
        };

        function setSearchNotification(params) {
            LOG.info( "Set search notification. Params: " +params );
            searchNotification = new SyncNotification(params, true);
            LOG.info( "Done Set search notification. Params: " +params );
        }

        function setSyncStatusListener(listener) {
                syncStatusListener = listener;
        }

        this.reportSyncStatus = function(status, errCode, details) {
            if (syncStatusListener != null
                    && (isReportingEnabled() || errCode == rho.errors.ERR_SYNCVERSION)) {
                if (errCode == rho.errors.ERR_SYNCVERSION) {
                    status = __getErrorText(errCode);
                } else {
                    details = details || __getErrorText(errCode);
                    status += (details ? __getMessageText("details")+details : "");
                }
                LOG.info("Status: " +status);
                rho.notify.byEvent(rho.events.STATUS_CHANGED, status, errCode);
            }
        };

/*
        void fireBulkSyncNotification( boolean bFinish, String status, String partition, int nErrCode )
        {
            if ( getSync().getState() == SyncEngine.esExit )
                return;

            if( nErrCode != RhoAppAdapter.ERR_NONE)
            {
                String strMessage = RhoAppAdapter.getMessageText("sync_failed_for") + "bulk.";
                reportSyncStatus(strMessage,nErrCode,"");
            }

            String strParams = "";
            strParams += "partition=" + partition;
            strParams += "&bulk_status="+status;
            strParams += "&sync_type=bulk";

            doFireSyncNotification( null, bFinish, nErrCode, "", strParams, "" );
        }
*/

        this.fireAllSyncNotifications = function(isFinish, errCode, error, serverError ) {
            if (engine.getState() == engine.states.exit) return;

            if(errCode != rho.errors.ERR_NONE) {
                if (!engine.isSearch()) {
                    var strMessage = __getMessageText("sync_failed_for") + "all.";
                    this.reportSyncStatus(strMessage,errCode,error);
                }
            }
            var sn = getSyncNotifyBySrc(null);
            if (sn) {
                this.doFireSyncNotification(null, isFinish, errCode, error, "", serverError);
            }
        };

        this.fireSyncNotification = function(src, isFinish, errCode, message ) {
            if (engine.getState() == engine.states.exit) return;

            if (message || errCode != rho.errors.ERR_NONE) {
                if (!engine.isSearch()) {
                    if (src != null && !message)
                        message = __getMessageText("sync_failed_for") + src.name + ".";

                    this.reportSyncStatus(message, errCode, src != null ? src.error : "");
                }
            }
            this.doFireSyncNotification(src, isFinish, errCode, "", "", "" );
        };

        function getSyncNotifyBySrc(src) {
            var sn = null; // sync notification
            if (engine.isSearch()) {
                sn = searchNotification;
            } else {
                if (src != null) sn = syncNotifications[src.id];
                if (sn == null) sn = allNotification;
            }
            if (sn == null && !engine.isNoThreadedMode()) return null;
            return sn != null ? sn : emptyNotify;
        }

        this.doFireSyncNotification = function(src, isFinish, errCode, error, params, serverError) {
            if (engine.isStoppedByUser()) return;

            try {
                var pSN = null;

                var strBody = "";
                var bRemoveAfterFire = isFinish;
                {
                    pSN = getSyncNotifyBySrc(src);
                    if (!pSN) return;

                    strBody = "";

                    if (src) {
                        strBody += "total_count=" + src.totalCount;
                        strBody += "&processed_count=" + src.curPageCount;
                        strBody += "&processed_objects_count=" + getLastSyncObjectCount(src.id);
                        strBody += "&cumulative_count=" + src.serverObjectsCount;
                        strBody += "&source_id=" + src.id;
                        strBody += "&source_name=" + src.name;
                    }

                    strBody += (strBody ? "&" : "") +(params || "sync_type=incremental");

                    strBody += "&status=";
                    if (isFinish) {
                        if (errCode == rho.errors.ERR_NONE) {
                            //if (engine.isSchemaChanged()) {
                            //    strBody += "schema_changed";
                            //} else {
                                strBody += (!src && !params) ? "complete" : "ok";
                            //}
                        } else {
                            if (engine.isStoppedByUser()) {
                                errCode = rho.errors.ERR_CANCELBYUSER;
                            }

                            strBody += "error";
                            strBody += "&error_code=" + errCode;

                            if (error) {
                                strBody += "&error_message=" + __urlEncode(error);
                            } else if (src) {
                                strBody += "&error_message=" + __urlEncode(src.error);
                            }

                            if (serverError) {
                                strBody += "&" + serverError;
                            } else if (src && src.serverError) {
                                strBody += "&" + src.serverError;
                            }
                        }

                        if (src) {
                            strBody += makeCreateObjectErrorBody(src.id);
                        }
                    } else {
                        strBody += "in_progress";
                    }

                    strBody += "&rho_callback=1";
                    /*
                    if (pSN.params) {
                        if (!pSN.params.match(/^&/)) {
                            strBody += "&";
                        }
                        strBody += pSN.params;
                    }
                    */

                    bRemoveAfterFire = bRemoveAfterFire && pSN.removeAfterFire;
                }
                if (bRemoveAfterFire) {
                    this.clearNotification(src);
                }
                LOG.info("Fire notification. Source: " +(src ? src.name : "") +"; " +pSN.toString());

                if (callNotify(pSN, strBody)) {
                    this.clearNotification(src);
                }
            } catch(exc) {
                LOG.error("Fire notification failed.", exc);
            }
        };

        function callNotify(oNotify, strBody) {
            if (engine.isNoThreadedMode()) {
                strNotifyBody = strBody;
                return false;
            }

            //TODO: implement real notification here!

            // let's try this as an implementation
            if (oNotify && "function" == typeof oNotify.params) {
                return oNotify.params();
            } else {
                return true;
            }

            //NetResponse resp = getNet().pushData( oNotify.m_strUrl, strBody, null );
            //if ( !resp.isOK() )
            //    LOG.error( "Fire object notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
            //else
            //{
            //    String szData = resp.getCharData();
            //    return szData != null && szData.equals("stop");
            //}

        }

        this.setNotification = function(src, notification) {
            if (!src) return;
            this.setSyncNotification(src.id, notification);
        };

        this.setSyncNotification = function(srcId, notification) {
            LOG.info("Set notification. Source ID: " +srcId +";" +(notification ? notification.toString() : ""));
            if (srcId == -1) {
                allNotification = notification;
            } else {
                syncNotifications[srcId] = notification;
            }
        };

        this.clearNotification = function(src) {
            LOG.info("Clear notification. Source: " +(src ? src.name : ""));
            if (engine.isSearch()) searchNotification = null;
            else syncNotifications[src.id] = null;
        };

        this.clearSyncNotification = function(srcId) {
            LOG.info("Clear notification. Source ID: " +srcId);
            if (srcId == -1) allNotification = null; //Clear all
            else syncNotifications[srcId] = null;
        };

        this.cleanLastSyncObjectCount = function() {
            hashSrcObjectCount = {};
        };

        this.incLastSyncObjectCount = function(srcId) {
            var nCount = hashSrcObjectCount[srcId] || 0;
            nCount += 1;

            hashSrcObjectCount[srcId] = nCount;

            return nCount || 0;
        };

        function getLastSyncObjectCount(srcId) {
            return hashSrcObjectCount[srcId] || 0;
        }


        this.callLoginCallback = function(oNotify, nErrCode, strMessage) {
            //try {
                if (engine.isStoppedByUser())
                    return;

                var strBody = "error_code=" + nErrCode;

                strBody += "&error_message=" + __urlEncode(strMessage != null? strMessage : "");
                strBody += "&rho_callback=1";

                LOG.info("Login callback: " +oNotify.toString() +". Body: " +strBody);

                callNotify(oNotify, strBody);
            //} catch (Exception exc) {
            //    LOG.error("Call Login callback failed.", exc);
            //}
        };

        function isReportingEnabled() {
            return enableReporting && enableReportingGlobal;
        }

    }

    function __getErrorText(key) {
        //TODO: to implement
        return key;
    }

    function __getMessageText(key) {
        //TODO: to implement
        return key;
    }

    function __urlEncode(value) {
        return value;
    }

    function notifyByEvent(type /*, arg1, arg2, ... argN*/) {
        $(window).trigger(jQuery.Event(type), $.makeArray(arguments).slice(1));
        // fire exact notifications here
    }

    $.extend(rho, {
        notify: publicInterface()
    });
    $.extend(RhoSync, {SyncNotification: SyncNotification});

})(jQuery);
