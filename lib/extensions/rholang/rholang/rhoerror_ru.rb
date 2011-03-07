module Rho
  class RhoError < Exception
    def self.err_message(code)
        if code == ERR_NETWORK
            return "Невозможно установить сетевое соединение."
        elsif code == ERR_REMOTESERVER
            return "Севрер вернул ошибку."
        elsif code ==  ERR_RUNTIME
            return "Произошла внутренняя ошибка приложения. Перезапустите приложение."
        elsif code ==  ERR_UNEXPECTEDSERVERRESPONSE
            return "Сервер синхронизации вернул даные в неверном формате."
        elsif code ==  ERR_DIFFDOMAINSINSYNCSRC
            return "Все sync sources должны быть из одного сетевого домена. Переустновите приложение."
        elsif code ==  ERR_NOSERVERRESPONSE
            return "Невозможно установить соединение с сервером синхронизации."
        elsif code ==  ERR_CLIENTISNOTLOGGEDIN
            return "Клиент не авторизован. Синхронизация данных не будет выполнена."
        elsif code ==  ERR_CUSTOMSYNCSERVER
            return "Невозможно установить соединение с сервером данных."
        elsif code ==  ERR_UNATHORIZED
            return "Вы ввели неверное имя пользователя/пароль, повторите попытку снова."
        elsif code ==  ERR_CANCELBYUSER
            return "Операция была отменена пользователем."
        elsif code ==  ERR_NONE
            return ""
        end
        
        return "Неизвестная ошибка."
    end
    
  end # RhoError
end # Rho
