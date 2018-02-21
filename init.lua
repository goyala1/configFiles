-- **** Hammerspoon config file. Coded in Lua. Window management shortcuts among others **** 

-- Example program to send a "Hello World" notification
hs.hotkey.bind({"cmd", "alt", "ctrl"}, "W", function()
      hs.notify.new({title="Hammerspoon", informativeText="Hello World"}):send()
end)

-- Alt+2 pushes currently active window to left side of screen
hs.hotkey.bind({"alt"}, "2", function()
      local win = hs.window.focusedWindow()
      local f = win:frame()
      local screen = win:screen()
      local max = screen:frame()

      f.x = max.x
      f.y = max.y
      f.w = max.w / 2
      f.h = max.h
      win:setFrame(f)
end)

-- Reload configuration quickly whenever file is saved automatically
function reloadConfig(files)
   doReload = false
   for _,file in pairs(files) do
      if file:sub(-4) == ".lua" then
	 doReload = true
      end
   end
   if doReload then
      hs.reload()
   end
end
myWatcher = hs.pathwatcher.new(os.getenv("HOME") .. "/.hammerspoon/", reloadConfig):start()
hs.alert.show("Config loaded")

caffeine = hs.menubar.new()
function setCaffeineDisplay(state)
   if state then
      caffeine:setTitle("AWAKE")
   else
      caffeine:setTitle("SLEEPY")
   end
end

function caffeineClicked()
   setCaffeineDisplay(hs.caffeinate.toggle("displayIdle"))
end

if caffeine then
   caffeine:setClickCallback(caffeineClicked)
   setCaffeineDisplay(hs.caffeinate.get("displayIdle"))
end

-- Wifi watcher. If leave homeSSID, do something. 
wifiWatcher = nil
homeSSID = "MZRT4" 
lastSSID = hs.wifi.currentNetwork()

function ssidChangedCallback()
   newSSID = hs.wifi.currentNetwork()

   if newSSID == homeSSID and lastSSID ~= homeSSID then
      -- We just joined our home WiFi network
      hs.audiodevice.defaultOutputDevice():setVolume(25)
   elseif newSSID ~= homeSSID and lastSSID == homeSSID then
      -- We just departed our home WiFi network
      hs.audiodevice.defaultOutputDevice():setVolume(0)
   end

   lastSSID = newSSID
end

wifiWatcher = hs.wifi.watcher.new(ssidChangedCallback)
wifiWatcher:start()

-- Send message when screen unlocks (replace with something more useful later) 
cWatcher = hs.caffeinate.watcher.new(function()
      if hs.caffeinate.watcher.screensDidUnlock then
	 --	 hs.notify.new({title="Your compeee", informativeText="Hi Abhi!"}):send()
      end
end) 
cWatcher:start()

-- hotkey shortcuts 
hs.hotkey.bind({"ctrl", "alt"}, 'L', function() hs.caffeinate.lockScreen() end)

hs.grid.setMargins({0, 0})
hs.hotkey.bind({'ctrl', 'alt'}, 'E', function () hs.application.launchOrFocus("Emacs") end)
hs.hotkey.bind({'ctrl', 'alt'}, 'G', function () hs.application.launchOrFocus("Google Chrome") end)
hs.hotkey.bind({'ctrl', 'alt'}, 'M', function () hs.application.launchOrFocus("Airmail 3") end) 
hs.hotkey.bind({'ctrl', 'alt'}, 'C', function () hs.application.launchOrFocus("Calendar") end) 
hs.hotkey.bind({'ctrl', 'alt'}, 'S', function () hs.hints.windowHints() end)


