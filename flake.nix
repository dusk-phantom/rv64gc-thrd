{
  description = "rv64gc-thrd - RISC-V 64位用户级线程库开发环境";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        riscv64Pkgs = pkgs.pkgsCross.riscv64;
        llvmPkgs = pkgs.llvmPackages_18;

        # RISC-V GCC（用于获取 libgcc 等运行时库）
        riscvGcc = riscv64Pkgs.stdenv.cc;

        # 构建完整的 sysroot 目录（包含 libgcc_s）
        riscvSysroot = pkgs.symlinkJoin {
          name = "riscv64-sysroot";
          paths = [
            riscv64Pkgs.glibc
            riscv64Pkgs.glibc.dev
            riscv64Pkgs.glibc.static
            riscv64Pkgs.linuxHeaders
            riscvGcc.cc.lib  # libgcc_s.so
          ];
        };
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = [
            # LLVM 工具链（unwrapped，支持 --target 交叉编译）
            llvmPkgs.clang-unwrapped
            llvmPkgs.llvm
            llvmPkgs.lld
            llvmPkgs.bintools  # llvm-ar 等

            # RISC-V GCC（提供 libgcc、crtbegin 等）
            riscvGcc

            # QEMU 用于运行 RISC-V 程序
            pkgs.qemu
            pkgs.bear

            # 调试工具
            pkgs.gdb
          ];

          # 设置路径
          RISCV_SYSROOT = "${riscvSysroot}";
          # clang 内置头文件路径（stddef.h 等）
          CLANG_RESOURCE_DIR = "${llvmPkgs.clang-unwrapped.lib}/lib/clang/18";
          # GCC 运行时库路径（crtbegin.o 等）
          RISCV_GCC_LIB = "${riscvGcc.cc}/lib/gcc/riscv64-unknown-linux-gnu/${riscvGcc.cc.version}";
          # libgcc_s.so 路径
          RISCV_GCC_LIBDIR = "${riscvGcc.cc.lib}/lib";

          shellHook = ''
            echo "rv64gc-thrd RISC-V 开发环境已加载"
            echo ""
            echo "交叉编译: make lib"
            echo "运行测试: make test4"
          '';
        };
      });
}
