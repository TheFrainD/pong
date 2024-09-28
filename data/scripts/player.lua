local Input = require("Input")
local System = require("System")

function Update(deltaTime)
	local speed = 250.0

	local transform = self.GetComponent("Transform")
	local sprite = self.GetComponent("Sprite")

	local position = transform.position

	local upKey
	local downKey

	if isPlayerOne then
		upKey = Input.Key.W
		downKey = Input.Key.S
	else
		upKey = Input.Key.Up
		downKey = Input.Key.Down
	end

	if Input.IsKeyDown(upKey) and position.y > 0 then
		position.y = position.y - speed * deltaTime
	elseif Input.IsKeyDown(downKey) and position.y < (System.Window.GetHeight() - sprite.size.y) then
		position.y = position.y + speed * deltaTime
	end
end

