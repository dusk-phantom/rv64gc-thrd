-- 设置目标名称
target("thrd")

    -- 设置目标类型为动态库
    set_kind("shared")

    -- 添加编译和链接的标志
    add_cflags("-Wall", "-fPIC", {public = true})
    add_includedirs("inc", {public = true})
    add_ldflags("-shared", {public = true})

    -- 添加源文件
    add_files("src/*.c")
    add_files("src/*.s")

    -- 设置生成的目标文件名和位置
    set_targetdir("build")
    set_basename("libthrd")

    -- 添加clean任务来清理构建目录
    on_clean(function (target)
        os.rmdir("build")
    end)

    -- 添加自定义debug配置（仅示例，需要根据实际调试工具自定义）
    target("debug")
        on_run(function ()
            os.exec("gdb ./build/libthrd.so")
        end)
