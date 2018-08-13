
local x, y, z = _getPosition()

---if (x > 1) then
---  _movePosition(-2, 0, 0)
---else
---  _movePosition(0.01, 0, 0)
---end
--_setPosition(8, -1, 7.5)
if x > 8 then 
  _setRotation(0, -1.5755, 0)
  _setPosition(8, -1, 7.5)
end
if x < -8 then 
  _setRotation(0, 1.5755, 0)  
  _setPosition(-8, -1, 7.5)
end
_moveForward(0.1)
--_lookAt(0, -1, 0)
--_movePosition(0, 0, 1)
--_moveRight(0.005)
_setRoughness(1)
_setMetalness(0.8)
