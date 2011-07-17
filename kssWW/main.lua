--Main code
local game = nil
local gui  = nil

--print("Hello from Lua")
--a = 10
--b = 20
--c = a + b

function main()
 --print('hello from lua')
end

function onAction(g, n, a)
 if( g == gui:class() ) then
  if( n == "btGame" ) then
   gui:show("gOptions", 0)
   gui:show("gGame", 1)
  elseif( n == "btOpts" )then
   gui:show("gGame", 0)
   gui:show("gOptions", 1)
  elseif( n == "btQuit" ) then
   game:quit()
  elseif( n == "lstMaps" ) then
   game:load(a)
  end
 end
end

game = Game()
gui  = Gui()

gui:load('main.gui')
game:addGui(gui:class())


game:start()
