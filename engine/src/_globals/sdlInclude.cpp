#include "_globals/sdlInclude.h"

std::string SDL_EventType_to_string( SDL_EventType val ) {
  switch( val ) {
    case SDL_EventType::SDL_QUIT:
      return "SDL_QUIT";
    case SDL_EventType::SDL_APP_TERMINATING:
      return "SDL_APP_TERMINATING";
    case SDL_EventType::SDL_APP_LOWMEMORY:
      return "SDL_APP_LOWMEMORY";
    case SDL_EventType::SDL_APP_WILLENTERBACKGROUND:
      return "SDL_APP_WILLENTERBACKGROUND";
    case SDL_EventType::SDL_APP_DIDENTERBACKGROUND:
      return "SDL_APP_DIDENTERBACKGROUND";
    case SDL_EventType::SDL_APP_WILLENTERFOREGROUND:
      return "SDL_APP_WILLENTERFOREGROUND";
    case SDL_EventType::SDL_APP_DIDENTERFOREGROUND:
      return "SDL_APP_DIDENTERFOREGROUND";
    case SDL_EventType::SDL_LOCALECHANGED:
      return "SDL_LOCALECHANGED";
    case SDL_EventType::SDL_DISPLAYEVENT:
      return "SDL_DISPLAYEVENT";
    case SDL_EventType::SDL_WINDOWEVENT:
      return "SDL_WINDOWEVENT";
    case SDL_EventType::SDL_SYSWMEVENT:
      return "SDL_SYSWMEVENT";
    case SDL_EventType::SDL_KEYDOWN:
      return "SDL_KEYDOWN";
    case SDL_EventType::SDL_KEYUP:
      return "SDL_KEYUP";
    case SDL_EventType::SDL_TEXTEDITING:
      return "SDL_TEXTEDITING";
    case SDL_EventType::SDL_TEXTINPUT:
      return "SDL_TEXTINPUT";
    case SDL_EventType::SDL_KEYMAPCHANGED:
      return "SDL_KEYMAPCHANGED";
    case SDL_EventType::SDL_TEXTEDITING_EXT:
      return "SDL_TEXTEDITING_EXT";
    case SDL_EventType::SDL_MOUSEMOTION:
      return "SDL_MOUSEMOTION";
    case SDL_EventType::SDL_MOUSEBUTTONDOWN:
      return "SDL_MOUSEBUTTONDOWN";
    case SDL_EventType::SDL_MOUSEBUTTONUP:
      return "SDL_MOUSEBUTTONUP";
    case SDL_EventType::SDL_MOUSEWHEEL:
      return "SDL_MOUSEWHEEL";
    case SDL_EventType::SDL_JOYAXISMOTION:
      return "SDL_JOYAXISMOTION";
    case SDL_EventType::SDL_JOYBALLMOTION:
      return "SDL_JOYBALLMOTION";
    case SDL_EventType::SDL_JOYHATMOTION:
      return "SDL_JOYHATMOTION";
    case SDL_EventType::SDL_JOYBUTTONDOWN:
      return "SDL_JOYBUTTONDOWN";
    case SDL_EventType::SDL_JOYBUTTONUP:
      return "SDL_JOYBUTTONUP";
    case SDL_EventType::SDL_JOYDEVICEADDED:
      return "SDL_JOYDEVICEADDED";
    case SDL_EventType::SDL_JOYDEVICEREMOVED:
      return "SDL_JOYDEVICEREMOVED";
    case SDL_EventType::SDL_JOYBATTERYUPDATED:
      return "SDL_JOYBATTERYUPDATED";
    case SDL_EventType::SDL_CONTROLLERAXISMOTION:
      return "SDL_CONTROLLERAXISMOTION";
    case SDL_EventType::SDL_CONTROLLERBUTTONDOWN:
      return "SDL_CONTROLLERBUTTONDOWN";
    case SDL_EventType::SDL_CONTROLLERBUTTONUP:
      return "SDL_CONTROLLERBUTTONUP";
    case SDL_EventType::SDL_CONTROLLERDEVICEADDED:
      return "SDL_CONTROLLERDEVICEADDED";
    case SDL_EventType::SDL_CONTROLLERDEVICEREMOVED:
      return "SDL_CONTROLLERDEVICEREMOVED";
    case SDL_EventType::SDL_CONTROLLERDEVICEREMAPPED:
      return "SDL_CONTROLLERDEVICEREMAPPED";
    case SDL_EventType::SDL_CONTROLLERTOUCHPADDOWN:
      return "SDL_CONTROLLERTOUCHPADDOWN";
    case SDL_EventType::SDL_CONTROLLERTOUCHPADMOTION:
      return "SDL_CONTROLLERTOUCHPADMOTION";
    case SDL_EventType::SDL_CONTROLLERTOUCHPADUP:
      return "SDL_CONTROLLERTOUCHPADUP";
    case SDL_EventType::SDL_CONTROLLERSENSORUPDATE:
      return "SDL_CONTROLLERSENSORUPDATE";
    case SDL_EventType::SDL_FINGERDOWN:
      return "SDL_FINGERDOWN";
    case SDL_EventType::SDL_FINGERUP:
      return "SDL_FINGERUP";
    case SDL_EventType::SDL_FINGERMOTION:
      return "SDL_FINGERMOTION";
    case SDL_EventType::SDL_DOLLARGESTURE:
      return "SDL_DOLLARGESTURE";
    case SDL_EventType::SDL_DOLLARRECORD:
      return "SDL_DOLLARRECORD";
    case SDL_EventType::SDL_MULTIGESTURE:
      return "SDL_MULTIGESTURE";
    case SDL_EventType::SDL_CLIPBOARDUPDATE:
      return "SDL_CLIPBOARDUPDATE";
    case SDL_EventType::SDL_DROPFILE:
      return "SDL_DROPFILE";
    case SDL_EventType::SDL_DROPTEXT:
      return "SDL_DROPTEXT";
    case SDL_EventType::SDL_DROPBEGIN:
      return "SDL_DROPBEGIN";
    case SDL_EventType::SDL_DROPCOMPLETE:
      return "SDL_DROPCOMPLETE";
    case SDL_EventType::SDL_AUDIODEVICEADDED:
      return "SDL_AUDIODEVICEADDED";
    case SDL_EventType::SDL_AUDIODEVICEREMOVED:
      return "SDL_AUDIODEVICEREMOVED";
    case SDL_EventType::SDL_SENSORUPDATE:
      return "SDL_SENSORUPDATE";
    case SDL_EventType::SDL_RENDER_TARGETS_RESET:
      return "SDL_RENDER_TARGETS_RESET";
    case SDL_EventType::SDL_RENDER_DEVICE_RESET:
      return "SDL_RENDER_DEVICE_RESET";
    case SDL_EventType::SDL_POLLSENTINEL:
      return "SDL_POLLSENTINEL";
    default:
      if( val <= SDL_EventType::SDL_USEREVENT && val > SDL_EventType::SDL_LASTEVENT ) {
        return "SDL_USEREVENT";
      }
      return "";
  }
}

std::string SDL_WindowFlags_to_string( SDL_WindowFlags flags ) {
  std::string ret = "";
  switch( flags ) {
    case SDL_WindowFlags::SDL_WINDOW_FULLSCREEN:
      ret += "SDL_WINDOW_FULLSCREEN | ";
    case SDL_WindowFlags::SDL_WINDOW_OPENGL:
      ret += "SDL_WINDOW_OPENGL | ";
    case SDL_WindowFlags::SDL_WINDOW_SHOWN:
      ret += "SDL_WINDOW_SHOWN | ";
    case SDL_WindowFlags::SDL_WINDOW_HIDDEN:
      ret += "SDL_WINDOW_HIDDEN | ";
    case SDL_WindowFlags::SDL_WINDOW_BORDERLESS:
      ret += "SDL_WINDOW_BORDERLESS | ";
    case SDL_WindowFlags::SDL_WINDOW_RESIZABLE:
      ret += "SDL_WINDOW_RESIZABLE | ";
    case SDL_WindowFlags::SDL_WINDOW_MINIMIZED:
      ret += "SDL_WINDOW_MINIMIZED | ";
    case SDL_WindowFlags::SDL_WINDOW_MAXIMIZED:
      ret += "SDL_WINDOW_MAXIMIZED | ";
    case SDL_WindowFlags::SDL_WINDOW_MOUSE_GRABBED:
      ret += "SDL_WINDOW_MOUSE_GRABBED | ";
    case SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS:
      ret += "SDL_WINDOW_INPUT_FOCUS | ";
    case SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS:
      ret += "SDL_WINDOW_MOUSE_FOCUS | ";
    case SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP:
      ret += "SDL_WINDOW_FULLSCREEN_DESKTOP | ";
    case SDL_WindowFlags::SDL_WINDOW_FOREIGN:
      ret += "SDL_WINDOW_FOREIGN | ";
    case SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI:
      ret += "SDL_WINDOW_ALLOW_HIGHDPI | ";
    case SDL_WindowFlags::SDL_WINDOW_MOUSE_CAPTURE:
      ret += "SDL_WINDOW_MOUSE_CAPTURE | ";
    case SDL_WindowFlags::SDL_WINDOW_ALWAYS_ON_TOP:
      ret += "SDL_WINDOW_ALWAYS_ON_TOP | ";
    case SDL_WindowFlags::SDL_WINDOW_SKIP_TASKBAR:
      ret += "SDL_WINDOW_SKIP_TASKBAR | ";
    case SDL_WindowFlags::SDL_WINDOW_UTILITY:
      ret += "SDL_WINDOW_UTILITY | ";
    case SDL_WindowFlags::SDL_WINDOW_TOOLTIP:
      ret += "SDL_WINDOW_TOOLTIP | ";
    case SDL_WindowFlags::SDL_WINDOW_POPUP_MENU:
      ret += "SDL_WINDOW_POPUP_MENU | ";
    case SDL_WindowFlags::SDL_WINDOW_KEYBOARD_GRABBED:
      ret += "SDL_WINDOW_KEYBOARD_GRABBED | ";
    case SDL_WindowFlags::SDL_WINDOW_VULKAN:
      ret += "SDL_WINDOW_VULKAN | ";
    case SDL_WindowFlags::SDL_WINDOW_METAL:
      ret += "SDL_WINDOW_METAL | ";
      // case SDL_WindowFlags::SDL_WINDOW_INPUT_GRABBED:
      //   ret += "SDL_WINDOW_INPUT_GRABBED | ";
      return ret.substr( 0, ret.size() - 3 );
    default:
      return "";
  }
}

std::string SDL_WindowEventID_to_string( SDL_WindowEventID val ) {
  switch( val ) {
    case SDL_WindowEventID::SDL_WINDOWEVENT_NONE:
      return "SDL_WINDOWEVENT_NONE";
    case SDL_WindowEventID::SDL_WINDOWEVENT_SHOWN:
      return "SDL_WINDOWEVENT_SHOWN";
    case SDL_WindowEventID::SDL_WINDOWEVENT_HIDDEN:
      return "SDL_WINDOWEVENT_HIDDEN";
    case SDL_WindowEventID::SDL_WINDOWEVENT_EXPOSED:
      return "SDL_WINDOWEVENT_EXPOSED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_MOVED:
      return "SDL_WINDOWEVENT_MOVED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED:
      return "SDL_WINDOWEVENT_RESIZED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
      return "SDL_WINDOWEVENT_SIZE_CHANGED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_MINIMIZED:
      return "SDL_WINDOWEVENT_MINIMIZED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_MAXIMIZED:
      return "SDL_WINDOWEVENT_MAXIMIZED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_RESTORED:
      return "SDL_WINDOWEVENT_RESTORED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_ENTER:
      return "SDL_WINDOWEVENT_ENTER";
    case SDL_WindowEventID::SDL_WINDOWEVENT_LEAVE:
      return "SDL_WINDOWEVENT_LEAVE";
    case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_GAINED:
      return "SDL_WINDOWEVENT_FOCUS_GAINED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_LOST:
      return "SDL_WINDOWEVENT_FOCUS_LOST";
    case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE:
      return "SDL_WINDOWEVENT_CLOSE";
    case SDL_WindowEventID::SDL_WINDOWEVENT_TAKE_FOCUS:
      return "SDL_WINDOWEVENT_TAKE_FOCUS";
    case SDL_WindowEventID::SDL_WINDOWEVENT_HIT_TEST:
      return "SDL_WINDOWEVENT_HIT_TEST";
    case SDL_WindowEventID::SDL_WINDOWEVENT_ICCPROF_CHANGED:
      return "SDL_WINDOWEVENT_ICCPROF_CHANGED";
    case SDL_WindowEventID::SDL_WINDOWEVENT_DISPLAY_CHANGED:
      return "SDL_WINDOWEVENT_DISPLAY_CHANGED";
    default:
      return "";
  }
}

std::string SDL_DisplayEventID_to_string( SDL_DisplayEventID val ) {
  switch( val ) {
    case SDL_DisplayEventID::SDL_DISPLAYEVENT_NONE:
      return "SDL_DISPLAYEVENT_NONE";
    case SDL_DisplayEventID::SDL_DISPLAYEVENT_ORIENTATION:
      return "SDL_DISPLAYEVENT_ORIENTATION";
    case SDL_DisplayEventID::SDL_DISPLAYEVENT_CONNECTED:
      return "SDL_DISPLAYEVENT_CONNECTED";
    case SDL_DisplayEventID::SDL_DISPLAYEVENT_DISCONNECTED:
      return "SDL_DISPLAYEVENT_DISCONNECTED";
    case SDL_DisplayEventID::SDL_DISPLAYEVENT_MOVED:
      return "SDL_DISPLAYEVENT_MOVED";
    default:
      return "";
  }
}

std::string SDL_DisplayOrientation_to_string( SDL_DisplayOrientation val ) {
  switch( val ) {
    case SDL_DisplayOrientation::SDL_ORIENTATION_UNKNOWN:
      return "SDL_ORIENTATION_UNKNOWN";
    case SDL_DisplayOrientation::SDL_ORIENTATION_LANDSCAPE:
      return "SDL_ORIENTATION_LANDSCAPE";
    case SDL_DisplayOrientation::SDL_ORIENTATION_LANDSCAPE_FLIPPED:
      return "SDL_ORIENTATION_LANDSCAPE_FLIPPED";
    case SDL_DisplayOrientation::SDL_ORIENTATION_PORTRAIT:
      return "SDL_ORIENTATION_PORTRAIT";
    case SDL_DisplayOrientation::SDL_ORIENTATION_PORTRAIT_FLIPPED:
      return "SDL_ORIENTATION_PORTRAIT_FLIPPED";
    default:
      return "";
  }
}

std::string SDL_FlashOperation_to_string( SDL_FlashOperation val ) {
  switch( val ) {
    case SDL_FlashOperation::SDL_FLASH_CANCEL:
      return "SDL_FLASH_CANCEL";
    case SDL_FlashOperation::SDL_FLASH_BRIEFLY:
      return "SDL_FLASH_BRIEFLY";
    case SDL_FlashOperation::SDL_FLASH_UNTIL_FOCUSED:
      return "SDL_FLASH_UNTIL_FOCUSED";
    default:
      return "";
  }
}

std::string SDL_GLattr_to_string( SDL_GLattr val ) {
  switch( val ) {
    case SDL_GLattr::SDL_GL_RED_SIZE:
      return "SDL_GL_RED_SIZE";
    case SDL_GLattr::SDL_GL_GREEN_SIZE:
      return "SDL_GL_GREEN_SIZE";
    case SDL_GLattr::SDL_GL_BLUE_SIZE:
      return "SDL_GL_BLUE_SIZE";
    case SDL_GLattr::SDL_GL_ALPHA_SIZE:
      return "SDL_GL_ALPHA_SIZE";
    case SDL_GLattr::SDL_GL_BUFFER_SIZE:
      return "SDL_GL_BUFFER_SIZE";
    case SDL_GLattr::SDL_GL_DOUBLEBUFFER:
      return "SDL_GL_DOUBLEBUFFER";
    case SDL_GLattr::SDL_GL_DEPTH_SIZE:
      return "SDL_GL_DEPTH_SIZE";
    case SDL_GLattr::SDL_GL_STENCIL_SIZE:
      return "SDL_GL_STENCIL_SIZE";
    case SDL_GLattr::SDL_GL_ACCUM_RED_SIZE:
      return "SDL_GL_ACCUM_RED_SIZE";
    case SDL_GLattr::SDL_GL_ACCUM_GREEN_SIZE:
      return "SDL_GL_ACCUM_GREEN_SIZE";
    case SDL_GLattr::SDL_GL_ACCUM_BLUE_SIZE:
      return "SDL_GL_ACCUM_BLUE_SIZE";
    case SDL_GLattr::SDL_GL_ACCUM_ALPHA_SIZE:
      return "SDL_GL_ACCUM_ALPHA_SIZE";
    case SDL_GLattr::SDL_GL_STEREO:
      return "SDL_GL_STEREO";
    case SDL_GLattr::SDL_GL_MULTISAMPLEBUFFERS:
      return "SDL_GL_MULTISAMPLEBUFFERS";
    case SDL_GLattr::SDL_GL_MULTISAMPLESAMPLES:
      return "SDL_GL_MULTISAMPLESAMPLES";
    case SDL_GLattr::SDL_GL_ACCELERATED_VISUAL:
      return "SDL_GL_ACCELERATED_VISUAL";
    case SDL_GLattr::SDL_GL_RETAINED_BACKING:
      return "SDL_GL_RETAINED_BACKING";
    case SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION:
      return "SDL_GL_CONTEXT_MAJOR_VERSION";
    case SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION:
      return "SDL_GL_CONTEXT_MINOR_VERSION";
    case SDL_GLattr::SDL_GL_CONTEXT_EGL:
      return "SDL_GL_CONTEXT_EGL";
    case SDL_GLattr::SDL_GL_CONTEXT_FLAGS:
      return "SDL_GL_CONTEXT_FLAGS";
    case SDL_GLattr::SDL_GL_CONTEXT_PROFILE_MASK:
      return "SDL_GL_CONTEXT_PROFILE_MASK";
    case SDL_GLattr::SDL_GL_SHARE_WITH_CURRENT_CONTEXT:
      return "SDL_GL_SHARE_WITH_CURRENT_CONTEXT";
    case SDL_GLattr::SDL_GL_FRAMEBUFFER_SRGB_CAPABLE:
      return "SDL_GL_FRAMEBUFFER_SRGB_CAPABLE";
    case SDL_GLattr::SDL_GL_CONTEXT_RELEASE_BEHAVIOR:
      return "SDL_GL_CONTEXT_RELEASE_BEHAVIOR";
    case SDL_GLattr::SDL_GL_CONTEXT_RESET_NOTIFICATION:
      return "SDL_GL_CONTEXT_RESET_NOTIFICATION";
    case SDL_GLattr::SDL_GL_CONTEXT_NO_ERROR:
      return "SDL_GL_CONTEXT_NO_ERROR";
    case SDL_GLattr::SDL_GL_FLOATBUFFERS:
      return "SDL_GL_FLOATBUFFERS";
    default:
      return "";
  }
}

std::string SDL_GLprofile_to_string( SDL_GLprofile flags ) {
  std::string ret = "";
  switch( flags ) {
    case SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE:
      ret += "SDL_GL_CONTEXT_PROFILE_CORE | ";
    case SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
      ret += "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY | ";
    case SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_ES:
      ret += "SDL_GL_CONTEXT_PROFILE_ES | ";
      return ret.substr( 0, ret.size() - 3 );
    default:
      return "";
  }
}

std::string SDL_GLcontextFlag_to_string( SDL_GLcontextFlag flags ) {
  std::string ret = "";
  switch( flags ) {
    case SDL_GLcontextFlag::SDL_GL_CONTEXT_DEBUG_FLAG:
      ret += "SDL_GL_CONTEXT_DEBUG_FLAG | ";
    case SDL_GLcontextFlag::SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG:
      ret += "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | ";
    case SDL_GLcontextFlag::SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG:
      ret += "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG | ";
    case SDL_GLcontextFlag::SDL_GL_CONTEXT_RESET_ISOLATION_FLAG:
      ret += "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG | ";
      return ret.substr( 0, ret.size() - 3 );
    default:
      return "";
  }
}

std::string SDL_GLcontextReleaseFlag_to_string( SDL_GLcontextReleaseFlag flags ) {
  std::string ret = "";
  switch( flags ) {
    case SDL_GLcontextReleaseFlag::SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE:
      ret += "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE | ";
    case SDL_GLcontextReleaseFlag::SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH:
      ret += "SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH | ";
      return ret.substr( 0, ret.size() - 3 );
    default:
      return "";
  }
}

std::string SDL_GLContextResetNotification_to_string( SDL_GLContextResetNotification flags ) {
  std::string ret = "";
  switch( flags ) {
    case SDL_GLContextResetNotification::SDL_GL_CONTEXT_RESET_NO_NOTIFICATION:
      ret += "SDL_GL_CONTEXT_RESET_NO_NOTIFICATION | ";
    case SDL_GLContextResetNotification::SDL_GL_CONTEXT_RESET_LOSE_CONTEXT:
      ret += "SDL_GL_CONTEXT_RESET_LOSE_CONTEXT | ";
      return ret.substr( 0, ret.size() - 3 );
    default:
      return "";
  }
}

std::string SDL_HitTestResult_to_string( SDL_HitTestResult val ) {
  switch( val ) {
    case SDL_HitTestResult::SDL_HITTEST_NORMAL:
      return "SDL_HITTEST_NORMAL";
    case SDL_HitTestResult::SDL_HITTEST_DRAGGABLE:
      return "SDL_HITTEST_DRAGGABLE";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_TOPLEFT:
      return "SDL_HITTEST_RESIZE_TOPLEFT";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_TOP:
      return "SDL_HITTEST_RESIZE_TOP";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_TOPRIGHT:
      return "SDL_HITTEST_RESIZE_TOPRIGHT";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_RIGHT:
      return "SDL_HITTEST_RESIZE_RIGHT";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_BOTTOMRIGHT:
      return "SDL_HITTEST_RESIZE_BOTTOMRIGHT";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_BOTTOM:
      return "SDL_HITTEST_RESIZE_BOTTOM";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_BOTTOMLEFT:
      return "SDL_HITTEST_RESIZE_BOTTOMLEFT";
    case SDL_HitTestResult::SDL_HITTEST_RESIZE_LEFT:
      return "SDL_HITTEST_RESIZE_LEFT";
    default:
      return "";
  }
}
