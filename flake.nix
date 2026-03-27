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

        riscvGcc = riscv64Pkgs.stdenv.cc;

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

        gccLibDir = "${riscvGcc.cc}/lib/gcc/riscv64-unknown-linux-gnu/${riscvGcc.cc.version}";
        gccRuntimeDir = "${riscvGcc.cc.lib}/lib";
        clangResourceDir = "${llvmPkgs.clang-unwrapped.lib}/lib/clang/18";

        # meson 交叉编译配置文件（所有 Nix store 路径在求值时确定）
        mesonCrossFile = pkgs.writeText "riscv64-cross.ini" ''
          [binaries]
          c = ['${llvmPkgs.clang-unwrapped}/bin/clang', '--target=riscv64-linux-gnu', '--sysroot=${riscvSysroot}', '-resource-dir', '${clangResourceDir}']
          c_ld = '${llvmPkgs.lld}/bin/ld.lld'
          ar = '${llvmPkgs.llvm}/bin/llvm-ar'
          strip = '${llvmPkgs.llvm}/bin/llvm-strip'
          exe_wrapper = ['${pkgs.qemu-user}/bin/qemu-riscv64', '-L', '${riscvSysroot}']

          [built-in options]
          c_link_args = ['-Wl,--sysroot=/', '--rtlib=libgcc', '-B${gccLibDir}', '-L${gccLibDir}', '-L${gccRuntimeDir}']

          [host_machine]
          system = 'linux'
          cpu_family = 'riscv64'
          cpu = 'riscv64'
          endian = 'little'
        '';
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

            # QEMU user-mode 用于运行 RISC-V 程序
            pkgs.qemu-user
            pkgs.bear

            # 构建系统
            pkgs.meson
            pkgs.ninja

            # 调试工具
            pkgs.gdb
          ];

          MESON_CROSS_FILE = "${mesonCrossFile}";

          shellHook = ''
            echo "rv64gc-thrd RISC-V 开发环境已加载"
            echo ""
            echo "初始化:   meson setup builddir --cross-file \$MESON_CROSS_FILE"
            echo "编译库:   ninja -C builddir"
            echo "运行测试: ninja -C builddir test"
          '';
        };
      });
}
