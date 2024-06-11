function setup()
  player = createGameObject()
  ground = createGameObject(1, -0.1, -0.5, 0, 11, 1, 1)
  bg = createGameObject(1, -0.095, -0.14, 0.0, 11.0, 5.915, 2)
end

setup()

gravity = -9.8
jumpStrength = 3.0


playerVelocityX = 0
playerVelocityY = 0
isOnGround = false 

KEY_A = 65
KEY_D = 68
KEY_W = 87
KEY_SHIFT = 340

function update(deltaTime)
  local x, y = getGameObjectPosition(player)
  local newX = x
  local newY = y
  local moveSpeed = 30.0 * deltaTime
  playerVelocityX = playerVelocityX * 0.95

  for k, v in pairs(pressedKeys) do
    if v == KEY_A then  
      playerVelocityX = playerVelocityX - moveSpeed
    end
    if v == KEY_D then  
      playerVelocityX = playerVelocityX + moveSpeed
    end
    if v == KEY_W and isOnGround then 
      playerVelocityY = jumpStrength
      isOnGround = false
    end
  end
  
  playerVelocityY = playerVelocityY + gravity * deltaTime

  newX = newX + playerVelocityX * deltaTime
  newY = newY + playerVelocityY * deltaTime

  if newY <= -0.64 then
    newY = -0.64
    playerVelocityY = 0
    isOnGround = true
  else
    isOnGround = false
  end

  setGameObjectPosition(player, newX, newY)

  drawGameObject(bg)
  drawGameObject(ground)
  drawGameObject(player)
end