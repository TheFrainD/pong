local System = require("System")

local SPEED = 100.0
local dx = -SPEED
local dy = 0

local transform
local scoreManager

local function resetPosition()
	transform.position.x = System.Window.GetWidth() / 2.0
	transform.position.y = System.Window.GetHeight() / 2.0
end

function OnStart()
	transform = self.GetComponent("Transform")
	scoreManager = GetEntity("ScoreManager").GetComponent("Score")
end

function OnCollision(other)
	dx = -dx * 1.03

	local playerPosition = other.GetComponent("Transform").position

	local name = other.GetName()
	if name == "Player1" then
		transform.position.x = playerPosition.x + other.GetComponent("Sprite").size.x
	else
		transform.position.x = playerPosition.x - self.GetComponent("Sprite").size.x
	end

	if dy < 0 then
		dy = -math.random(10, 150)
	else
		dy = math.random(10, 150)
	end
end

function Update(deltaTime)
	transform.position.x = transform.position.x + dx * deltaTime

	if transform.position.x < 0 then
		scoreManager.SetPlayerTwoScore(scoreManager.Score.player_one + 1)
		resetPosition()
	elseif transform.position.x > System.Window.GetWidth() then
		scoreManager.SetPlayerOneScore(scoreManager.Score.player_one + 1)
		resetPosition()
	end

	transform.position.y = transform.position.y + dy * deltaTime

	local topBorder = 0
	local bottomBorder = System.Window.GetHeight() - self.GetComponent("Sprite").size.y

	if transform.position.y < topBorder or transform.position.y > bottomBorder then
		dy = -dy
	end
end
