
local x, y, z = _getPosition()

---if (x > 1) then
---  _movePosition(-2, 0, 0)
---else
---  _movePosition(0.01, 0, 0)
---end
if z > 7.5 then 
  _setRotation(0, 3.12, 0)
  _setPosition(9, -1, 7.5)
end
if z < -7.5 then 
  _setRotation(0, -31.42, 0)
  _setPosition(9, -1, -7.3)
end
_moveForward(0.1)
--_lookAt(0, -1, 0)
--_movePosition(0, 0, 1)
--_moveRight(0.005)
_setRoughness(1)
_setMetalness(0.8)
