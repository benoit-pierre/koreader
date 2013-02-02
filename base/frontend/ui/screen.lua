--[[
    Copyright (C) 2011 Hans-Werner Hilse <hilse@web.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
]]--

--[[
Codes for rotation modes:

1 for no rotation, 
2 for landscape with bottom on the right side of screen, etc.

           2
   +--------------+
   | +----------+ |
   | |          | |
   | | Freedom! | |
   | |          | |  
   | |          | |  
 3 | |          | | 1
   | |          | |
   | |          | |
   | +----------+ |
   |              |
   |              |
   +--------------+
          0
--]]


Screen = {
	width = 0,
	height = 0,
	pitch = 0,
	native_rotation_mode = nil,
	cur_rotation_mode = 0,

	bb = nil,
	saved_bb = nil,

	fb = einkfb.open("/dev/fb0"),
}

function Screen:init()
	_, self.height = self.fb:getSize()
	-- for unknown strange reason, pitch*2 is less than screen width in KPW
	-- so we need to calculate width by pitch here
	self.width = self.fb:getPitch()*2
	self.pitch = self:getPitch()
	self.bb = Blitbuffer.new(self.width, self.height, self.pitch)
	self.native_rotation_mode = self.fb:getOrientation()
	self.cur_rotation_mode = self.native_rotation_mode
end

function Screen:refresh(refesh_type)
	if self.native_rotation_mode ==  self.cur_rotation_mode then
		self.fb.bb:blitFrom(self.bb, 0, 0, 0, 0, self.width, self.height)
	elseif self.native_rotation_mode == 0 and self.cur_rotation_mode == 1 then
		self.fb.bb:blitFromRotate(self.bb, 270)
	end
	self.fb:refresh(refesh_type)
end

-- @orien: 1 for clockwise rotate, -1 for anti-clockwise
-- Remember to reread screen resolution after this function call
-- WARNING: this method is deprecated!!! use setRotationMode() or
-- setViewMode() instead.
function Screen:screenRotate(orien)
	if orien == "clockwise" then
		orien = -1
	elseif orien == "anticlockwise" then
		orien = 1
	else
		return
	end

	self.cur_rotation_mode = (self.cur_rotation_mode + orien) % 4
	-- you have to reopen framebuffer after rotate
	self.fb:setOrientation(self.cur_rotation_mode)
	self.fb:close()
	self.fb = einkfb.open("/dev/fb0")
	Input.rotation = self.cur_rotation_mode
end

function Screen:getSize()
	return Geom:new{w = self.width, h = self.height}
end

function Screen:getWidth()
	return self.width
end

function Screen:getHeight()
	return self.height
end

function Screen:getPitch()
	return self.ptich
end

function Screen:updateRotationMode()
	-- in EMU mode, you will always get 0 from getOrientation()
	self.cur_rotation_mode = self.fb:getOrientation()
end

function Screen:setRotationMode(mode)
	-- mode 0 and mode 2 has the same width and height, so do mode 1 and 3
	if (self.cur_rotation_mode % 2) ~= (mode % 2) then
		self.width, self.height = self.height, self.width
	end
	self.cur_rotation_mode = mode
	self.bb:free()
	self.pitch = self.width/2
	self.bb = Blitbuffer.new(self.width, self.height, self.pitch)
end

function Screen:setViewMode(mode)
	if mode == "portrait" then
		if self.cur_rotation_mode ~= 0 then
			self:setRotationMode(0)
		end
	elseif mode == "landscape" then
		if self.cur_rotation_mode ~= 1 then
			self:setRotationMode(1)
		end
	end
end


--[[
  @brief change gesture's x and y coordinates according to screen view mode

  @param ges gesture that you want to adjust
  @return adjusted gesture.
--]]
function Screen:adjustGesCoordinate(ges)
	-- we do nothing is screen is not rotated
	if self.native_rotation_mode == self.cur_rotation_mode then
		return ges
	end

	if self.native_rotation_mode == 0 and self.cur_rotation_mode == 1 then
		ges.pos.x, ges.pos.y = (self.width - ges.pos.y), (ges.pos.x)
	end

	return ges
end

function Screen:saveCurrentBB()
	local width, height = self:getWidth(), self:getHeight()

	if not self.saved_bb then
		self.saved_bb = Blitbuffer.new(width, height, self:getPitch())
	end
	if self.saved_bb:getWidth() ~= width then
		self.saved_bb:free()
		self.saved_bb = Blitbuffer.new(width, height, self:getPitch())
	end
	self.saved_bb:blitFullFrom(self.fb.bb)
end

function Screen:restoreFromSavedBB()
	self:restoreFromBB(self.saved_bb)
end

function Screen:getCurrentScreenBB()
	local bb = Blitbuffer.new(self:getWidth(), self:getHeight())
	bb:blitFullFrom(self.fb.bb)
	return bb
end

function Screen:restoreFromBB(bb)
	if bb then
		self.fb.bb:blitFullFrom(bb)
	else
		DEBUG("Got nil bb in restoreFromSavedBB!")
	end
end
