module Rho
module AsyncHttp

    def self.process_auth_header(args)
        auth = args[:authentication] if args.has_key?(:authentication)
        auth = args [:authorization] if args.has_key?(:authorization)

        if ( auth && auth.is_a?( Hash ) && auth[:type] )
            args[:authType] = auth[:type]
            args[:authUser] = auth[:username]
            args[:authPassword] = auth[:password]        
        end
                
        args.delete(:authentication)
        args.delete(:authorization)
    end

    def self.preprocess_headers(args)
        process_auth_header(args)
        
        if args.has_key?(:ssl_verify_peer)
            val = args[:ssl_verify_peer]
            if !val.is_a? TrueClass and !val.is_a? FalseClass
                val = val ? true : false
            end
            args[:verifyPeerCertificate] = val
            args.delete(:ssl_verify_peer)
        end

        if args.has_key?( :http_command )        
            args[:httpVerb] = args[:http_command]
            args.delete(:http_command)
        end

        if args.has_key?( :file_content_type )        
            args[:fileContentType] = args[:file_content_type]
            args.delete(:file_content_type)
        end
        
        if args.has_key?( :multipart ) && args[:multipart]
            args[:multipart].each do |multipart_item|
                
                if multipart_item.has_key?( :content_type )
                    multipart_item[:contentType] = multipart_item[:content_type]
                    multipart_item.delete(:content_type)
                end
                
                if multipart_item.has_key?( :filename_base )
                    multipart_item[:filenameBase] = multipart_item[:filename_base]
                    multipart_item.delete(:filename_base)
                end
            end
        end
    end

    def self.get(args)
        preprocess_headers(args)
        if ( args[:callback] )
            Rho::NetworkAccess.get( args, args[:callback], args[:callback_param] )
        else
            Rho::NetworkAccess.get( args )
        end    
    end
    
    def self.post(args)
        preprocess_headers(args)
        if ( args[:callback] )
            Rho::NetworkAccess.post( args, args[:callback], args[:callback_param] )
        else
            Rho::NetworkAccess.post( args )
        end    
    end
    
    def self.download_file(args)
        preprocess_headers(args)
        if ( args[:callback] )
            Rho::NetworkAccess.downloadFile( args, args[:callback], args[:callback_param] )
        else
            Rho::NetworkAccess.downloadFile( args )
        end    
    end
    
    def self.upload_file(args)
        preprocess_headers(args)
        if ( args[:callback] )
            Rho::NetworkAccess.uploadFile( args, args[:callback], args[:callback_param] )
        else
            Rho::NetworkAccess.uploadFile( args )
        end    
    end

end
end