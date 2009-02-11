package com.rhomobile.rhodes.http;

public interface IHttpHandler {

    /** 
     * Handle request
     * @param request The HTTP requests to be handled
     * @param response The HTTP response to be used.
     */
    public void handle(HttpRequest request, HttpResponse response);
    
    /** 
     * Translate a path string.
     * @return the translated path
     */
    public String translate(String path);        
         
    /** 
     * Set server.
     * This method will be called when during HttpServer setup
     */
    public void setServer(HttpServer server) throws Exception;

    /** 
     * Destroy Handler.
     * Called by HttpServer.stop().
     */
    public void destroy();

    /**
     * Get Handler name
     * @return
     */
    public String getName();
}
