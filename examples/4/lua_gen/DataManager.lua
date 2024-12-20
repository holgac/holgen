-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class DataManager
---@field characters Character[]
---@field armors Armor[]
---@field weapons Weapon[]
DataManager = {}

---@param key string
---@return Character
function DataManager:GetCharacterFromName(key) end

---@param key string
---@return Armor
function DataManager:GetArmorFromName(key) end

---@param key string
---@return Weapon
function DataManager:GetWeaponFromName(key) end

---@param elem Character
---@return Character
function DataManager:AddCharacter(elem) end

---@param elem Armor
---@return Armor
function DataManager:AddArmor(elem) end

---@param elem Weapon
---@return Weapon
function DataManager:AddWeapon(elem) end

---@param idx number
---@return Character
function DataManager:GetCharacter(idx) end

---@param idx number
---@return Armor
function DataManager:GetArmor(idx) end

---@param idx number
---@return Weapon
function DataManager:GetWeapon(idx) end

---@param idx number
function DataManager:DeleteCharacter(idx) end

---@param idx number
function DataManager:DeleteArmor(idx) end

---@param idx number
function DataManager:DeleteWeapon(idx) end

---@return number
function DataManager:GetCharacterCount() end

---@return number
function DataManager:GetArmorCount() end

---@return number
function DataManager:GetWeaponCount() end
