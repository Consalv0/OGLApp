local x, y, z = _getPosition()
local i = _getLightIntensity()

if (y > 4) then
  _setPosition(3, 0, 3)
  _setLightIntensity(0.5)
  _castShadows(true)
else
  _movePosition(-0.001, 0.001, -0.001)
  _setLightIntensity(i + 0.008 )
  _rotate(0.005, -0.01, 0)
end

_lookAt(0, 0, 0)
_setAmbientLight(i / 10, i / 10, i / 10)