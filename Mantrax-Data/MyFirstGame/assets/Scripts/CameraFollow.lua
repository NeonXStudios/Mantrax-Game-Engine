local follow_speed = 3.0
local offset_camera = vector3.new(0, 1.0, -1.0)

function onTick()
    local cam = GameCamera

    if cam ~= nil then
        self:transform().position = mix_vec3(self:transform().position, Scene:GetObjectPerTag("Player"):transform().position + offset_camera, follow_speed * Time.DeltaTime())
    end
end