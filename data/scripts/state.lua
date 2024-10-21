local Input = require("Input")

State = {
	score_manager = nil,
	player_one = nil,
	player_two = nil,
	ball = nil,
}

function State:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function State:enter(deltaTime) end

function State:exit(deltaTime) end

function State:update(deltaTime) end

ServingState = State:new()
function ServingState:update(deltaTime)
	if Input.IsKeyDown(Input.Key.Space) then
		ChangeState(PlayingState)
	end
end

PlayingState = State:new()
function PlayingState:update(deltaTime)
	self.player_one.update(deltaTime)
	self.player_two.update(deltaTime)
	self.ball.update(deltaTime)
end

CurrentState = nil

function OnStart()
	CurrentState = ServingState:new({
		score_manager = GetEntity("ScoreManager").GetComponent("Score"),
		player_one = GetEntity("Player1").GetComponent("Player"),
		player_two = GetEntity("Player2").GetComponent("Player"),
		ball = GetEntity("Ball").GetComponent("Ball"),
	})
end

function Update(deltaTime)
	if CurrentState then
		CurrentState:update(deltaTime)
	end
end

function ChangeState(Target)
	CurrentState:exit()
	CurrentState = Target:new({
		score_manager = GetEntity("ScoreManager").GetComponent("Score"),
		player_one = GetEntity("Player1").GetComponent("Player"),
		player_two = GetEntity("Player2").GetComponent("Player"),
		ball = GetEntity("Ball").GetComponent("Ball"),
	})
	CurrentState:enter()
end
