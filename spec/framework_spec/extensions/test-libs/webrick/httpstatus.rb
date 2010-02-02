#
# httpstatus.rb -- HTTPStatus Class
#
# Author: IPR -- Internet Programming with Ruby -- writers
# Copyright (c) 2000, 2001 TAKAHASHI Masayoshi, GOTOU Yuuzou
# Copyright (c) 2002 Internet Programming with Ruby writers. All rights
# reserved.
#
# $IPR: httpstatus.rb,v 1.11 2003/03/24 20:18:55 gotoyuzo Exp $

module WEBrick

  module HTTPStatus

    class Status      < StandardError; end
    class Info        < Status; end
    class Success     < Status; end
    class Redirect    < Status; end
    class Error       < Status; end
    class ClientError < Error; end
    class ServerError < Error; end
    
    class EOFError < StandardError; end

    StatusMessage = {
      100 => 'Continue',
      101 => 'Switching Protocols',
      200 => 'OK',
      201 => 'Created',
      202 => 'Accepted',
      203 => 'Non-Authoritative Information',
      204 => 'No Content',
      205 => 'Reset Content',
      206 => 'Partial Content',
      300 => 'Multiple Choices',
      301 => 'Moved Permanently',
      302 => 'Found',
      303 => 'See Other',
      304 => 'Not Modified',
      305 => 'Use Proxy',
      307 => 'Temporary Redirect',
      400 => 'Bad Request',
      401 => 'Unauthorized',
      402 => 'Payment Required',
      403 => 'Forbidden',
      404 => 'Not Found',
      405 => 'Method Not Allowed',
      406 => 'Not Acceptable',
      407 => 'Proxy Authentication Required',
      408 => 'Request Timeout',
      409 => 'Conflict',
      410 => 'Gone',
      411 => 'Length Required',
      412 => 'Precondition Failed',
      413 => 'Request Entity Too Large',
      414 => 'Request-URI Too Large',
      415 => 'Unsupported Media Type',
      416 => 'Request Range Not Satisfiable',
      417 => 'Expectation Failed',
      500 => 'Internal Server Error',
      501 => 'Not Implemented',
      502 => 'Bad Gateway',
      503 => 'Service Unavailable',
      504 => 'Gateway Timeout',
      505 => 'HTTP Version Not Supported'
    }

    CodeToError = {}

    class InternalServerError < ServerError
      def code() 500 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[500] = const_get('InternalServerError')

    class PreconditionFailed < ClientError
      def code() 412 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[412] = const_get('PreconditionFailed')

    class Unauthorized < ClientError
      def code() 401 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[401] = const_get('Unauthorized')

    class Found < Redirect
      def code() 302 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[302] = const_get('Found')

    class NonAuthoritativeInformation < Success
      def code() 203 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[203] = const_get('NonAuthoritativeInformation')

    class ProxyAuthenticationRequired < ClientError
      def code() 407 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[407] = const_get('ProxyAuthenticationRequired')

    class NotImplemented < ServerError
      def code() 501 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[501] = const_get('NotImplemented')

    class RequestEntityTooLarge < ClientError
      def code() 413 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[413] = const_get('RequestEntityTooLarge')

    class PaymentRequired < ClientError
      def code() 402 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[402] = const_get('PaymentRequired')

    class SeeOther < Redirect
      def code() 303 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[303] = const_get('SeeOther')

    class NoContent < Success
      def code() 204 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[204] = const_get('NoContent')

    class RequestTimeout < ClientError
      def code() 408 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[408] = const_get('RequestTimeout')

    class Continue < Info
      def code() 100 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[100] = const_get('Continue')

    class BadGateway < ServerError
      def code() 502 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[502] = const_get('BadGateway')

    class RequestURITooLarge < ClientError
      def code() 414 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[414] = const_get('RequestURITooLarge')

    class Forbidden < ClientError
      def code() 403 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[403] = const_get('Forbidden')

    class NotModified < Redirect
      def code() 304 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[304] = const_get('NotModified')

    class ResetContent < Success
      def code() 205 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[205] = const_get('ResetContent')

    class Conflict < ClientError
      def code() 409 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[409] = const_get('Conflict')

    class OK < Success
      def code() 200 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[200] = const_get('OK')

    class SwitchingProtocols < Info
      def code() 101 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[101] = const_get('SwitchingProtocols')

    class ServiceUnavailable < ServerError
      def code() 503 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[503] = const_get('ServiceUnavailable')

    class UnsupportedMediaType < ClientError
      def code() 415 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[415] = const_get('UnsupportedMediaType')

    class NotFound < ClientError
      def code() 404 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[404] = const_get('NotFound')

    class UseProxy < Redirect
      def code() 305 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[305] = const_get('UseProxy')

    class PartialContent < Success
      def code() 206 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[206] = const_get('PartialContent')

    class Gone < ClientError
      def code() 410 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[410] = const_get('Gone')

    class MultipleChoices < Redirect
      def code() 300 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[300] = const_get('MultipleChoices')

    class Created < Success
      def code() 201 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[201] = const_get('Created')

    class GatewayTimeout < ServerError
      def code() 504 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[504] = const_get('GatewayTimeout')

    class RequestRangeNotSatisfiable < ClientError
      def code() 416 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[416] = const_get('RequestRangeNotSatisfiable')

    class MethodNotAllowed < ClientError
      def code() 405 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[405] = const_get('MethodNotAllowed')

    class LengthRequired < ClientError
      def code() 411 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[411] = const_get('LengthRequired')

    class BadRequest < ClientError
      def code() 400 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[400] = const_get('BadRequest')

    class MovedPermanently < Redirect
      def code() 301 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[301] = const_get('MovedPermanently')

    class Accepted < Success
      def code() 202 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[202] = const_get('Accepted')

    class HTTPVersionNotSupported < ServerError
      def code() 505 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[505] = const_get('HTTPVersionNotSupported')

    class ExpectationFailed < ClientError
      def code() 417 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[417] = const_get('ExpectationFailed')

    class NotAcceptable < ClientError
      def code() 406 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[406] = const_get('NotAcceptable')

    class TemporaryRedirect < Redirect
      def code() 307 end
      def reason_phrase() StatusMessage[code] end
      alias to_i code
    end

    CodeToError[307] = const_get('TemporaryRedirect')


    def reason_phrase(code)
      StatusMessage[code.to_i]
    end
    def info?(code)
      code.to_i >= 100 and code.to_i < 200
    end
    def success?(code)
      code.to_i >= 200 and code.to_i < 300
    end
    def redirect?(code)
      code.to_i >= 300 and code.to_i < 400
    end
    def error?(code)
      code.to_i >= 400 and code.to_i < 600
    end
    def client_error?(code)
      code.to_i >= 400 and code.to_i < 500
    end
    def server_error?(code)
      code.to_i >= 500 and code.to_i < 600
    end

    def self.[](code)
      CodeToError[code]
    end

    module_function :reason_phrase
    module_function :info?, :success?, :redirect?, :error?
    module_function :client_error?, :server_error?
  end
end
