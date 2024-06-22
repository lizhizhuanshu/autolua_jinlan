
Name = "autolua2"
Version = "0.0.1"

---@class JinLanPointColor
---@field x integer
---@field y integer
---@field color integer

---@alias JinLanFeature JinLanPointColor[]

if not GetFeature then
  ---获取颜色特征
  ---@return JinLanFeature
  function GetFeature() error("Unimplemented method") end
end

if not SetClipboard then
  ---设置剪贴板内容
  ---@param text string
  function SetClipboard(text) error("Unimplemented method") end
end

if not GetSelectedRect then
  ---@return integer, integer, integer, integer
  function GetSelectedRect() error("Unimplemented method") end
end

local function toColorStr(nColor)
  return string.format("%06X", nColor)
end

local function toAutoLuaFeature(x,y,color)
  return string.format("%d|%d|%s", x, y, toColorStr(color))
end

local function str(s)
  return string.format("%q", s)
end

Functions = {
  {
    name = "isFeature",
    method = function ()
      local o = GetFeature()
      if #o<=0 then return end
      local t = {}
      for i, value in ipairs(o) do
        t[i] = toAutoLuaFeature(o[i].x, o[i].y, o[i].color)
      end
      SetClipboard(str(table.concat(t, ",")))
    end
  },
  {
    name = "findFeature",
    method = function ()
      local f = GetFeature()
      if #f <=0 then return end
      local origin = f[1]
      local t = {toAutoLuaFeature(0,0, origin.color)}
      for i=2, #f do
        local p = f[i]
        t[i] = toAutoLuaFeature(p.x - origin.x, p.y - origin.y, p.color)
      end
      SetClipboard(str(table.concat(t, ",")))
    end
  }
}
