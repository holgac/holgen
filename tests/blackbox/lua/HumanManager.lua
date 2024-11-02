-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class HumanManagerMeta
---@field humans table<number, HumanMeta>
HumanManagerMeta = {}

---@param key string
---@return HumanMeta
function HumanManagerMeta:GetHumanFromName(key) end

---@param elem HumanMeta
---@return HumanMeta
function HumanManagerMeta:AddHuman(elem) end

---@param idx number
---@return HumanMeta
function HumanManagerMeta:GetHuman(idx) end

---@param key number
---@return bool
function HumanManagerMeta:HasHuman(key) end

---@param key number
function HumanManagerMeta:DeleteHuman(key) end

---@return number
function HumanManagerMeta:GetHumanCount() end