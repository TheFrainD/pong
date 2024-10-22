Score = {
	player_one = 0,
	player_two = 0,
}

local labels = {
	player_one = nil,
	player_two = nil,
}

local state = nil

local MAX_SCORE = 5

function Init()
	labels.player_one = GetEntity("Player1Score").GetComponent("Label")
	labels.player_two = GetEntity("Player2Score").GetComponent("Label")
	state = GetEntity("State").GetComponent("State")
end

function SetPlayerOneScore(score)
	Score.player_one = score
	labels.player_one.text = tostring(score)
	if score >= MAX_SCORE then
		state.ChangeState(state.ServingState:new({ serve = 2 }))
	end
end

function SetPlayerTwoScore(score)
	Score.player_two = score
	labels.player_two.text = tostring(score)
	if score >= MAX_SCORE then
		state.ChangeState(state.ServingState:new({ serve = 1 }))
	end
end

function Reset()
	SetPlayerOneScore(0)
	SetPlayerTwoScore(0)
end
