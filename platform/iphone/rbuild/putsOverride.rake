def puts(o=nil)
    if o != nil
        $stdout.puts(o)
        if $out_file_buf_enable
            $out_file_buf << o.to_s
        end
    end
end
