#pragma once

#include "../runtime/<%= $cur_module.name %>Runtime.h"
<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

namespace <%= $cur_module.name %>Runtime {
    public ref class C<%= $cur_module.name %>MethodResultImpl sealed: public IMethodResult
    {
        rho::apiGenerator::CMethodResult* oResult;
    public:
        C<%= $cur_module.name %>MethodResultImpl(int64 native);

        virtual void set(Platform::String^ res);
        virtual void set(Windows::Foundation::Collections::IVectorView<Platform::String^>^ res);
        virtual void set(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ res);
    };
}
<% $cur_module.parents.each do |parent| %>
}<%
end %>
