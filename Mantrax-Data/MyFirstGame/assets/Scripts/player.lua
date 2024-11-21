local gravity = 1.981
local walk_speed = 5.0
local jump_force = 25.0

function onTick()
    
    local controller = self:GetCharacterController()
    local animator = self:GetAnimator()

    
    if controller ~= nil then
        -- Obtener la cámara
        local camera = GameCamera
        

        local cameraForward = normalize(camera:Forward())
        local cameraRight = normalize(camera:Right())

        
        -- Movimiento basado en las teclas WASD
        local axis = GetAxis(65, 68) * walk_speed * Time.DeltaTime()         -- A y D
        local axisVertical = GetAxis(87, 83) * walk_speed * Time.DeltaTime() -- W y S
        
        -- Calcular la dirección del movimiento en función de la cámara
        local move_dir = cameraRight * axis + cameraForward * axisVertical
        move_dir.y = move_dir.y + -gravity * Time.DeltaTime()
        
        
        if (axis == 0 and axisVertical == 0) then
            animator:SetState("Idle")
        else
            animator:SetState("Walk")
        end
        
        -- Dependiendo a donde se caminer se cambiara la escala de X
        if (axis > 0) then
            self:GetTransform().Scale.x = 1;
        else
            self:GetTransform().Scale.x = -1;
        end
        
        -- Aplicar salto si se presiona la barra espaciadora (tecla 32)
        if OnKeyDown(32) then
            move_dir.y = move_dir.y + jump_force * Time.DeltaTime()
        end


        -- Mover el personaje
        controller:move(move_dir)
    end
end
