#pragma once
// This file will be auto-generated later.
#include "../holgen.h"

#include "nethost.h"
#include "coreclr_delegates.h"
#include "hostfxr.h"

#include <filesystem>
#include <vector>
#include <map>
#include <cassert>

#ifdef WINDOWS
#include <Windows.h>
#define STR(s) L##s
#define CH(c) L##c
#define DIR_SEPARATOR L'\\'
#define string_compare wcscmp
#else
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX
#define string_compare strcmp
#endif

/*
 * This is temporarily in the tests directory; it will be an optionally auto-generated file later.
 */
class DotNetHost {
  struct DotNetModule {
    hostfxr_handle mHostFXRHandle = nullptr;
    load_assembly_fn mHostFXRDelegate_load_assembly = nullptr;
    get_function_pointer_fn mHostFXRDelegate_get_function_pointer = nullptr;
  };

public:
  // Unlike the rest of the file, this function will not be auto-generated. The logic in it is tied
  // to the project structure so it's the responsibility of the user code.
  void Start();

  /**
   * In a typical game, there's at least one base/native module, and multiple optional modules that
   * could be defined by the developer or a modder.
   *
   * rootPath: Path to the base module.
   * modules: List of modules to load.
   */
  void Initialize(const std::filesystem::path &baseModule,
                  const std::vector<std::filesystem::path> &modules);

private:
#if WINDOWS
  HMODULE mHostFXRModule;
#else
  // We're keeping a handle here but there's no need; dlclose'ing it is UB
  void *mHostFXRModule = nullptr;
#endif
  hostfxr_initialize_for_dotnet_command_line_fn mHostFXR_initialize_for_dotnet_command_line =
      nullptr;
  hostfxr_initialize_for_runtime_config_fn mHostFXR_initialize_for_runtime_config = nullptr;
  hostfxr_get_runtime_delegate_fn mHostFXR_get_runtime_delegate = nullptr;
  hostfxr_close_fn mHostFXR_close = nullptr;
  std::map<std::string, DotNetModule> mModules;

  void *LoadLibrary(const char_t *path);

  template <typename T>
  void LoadFunction(void *dl, const char *name, T &result) {
    result = (T)LoadFunction(dl, name);
  }

  template <typename T>
  void LoadDelegate(hostfxr_handle hostFXRHandle, hostfxr_delegate_type delegateType, T &result) {
    result = (T)LoadDelegate(hostFXRHandle, delegateType);
  }

  void *LoadFunction(void *dl, const char *name);
  void *LoadDelegate(hostfxr_handle hostFXRHandle, hostfxr_delegate_type delegateType);

  void LoadModule(const std::filesystem::path &modulePath);
  void InitializeModule(const std::string &moduleName, DotNetModule &module);
};
