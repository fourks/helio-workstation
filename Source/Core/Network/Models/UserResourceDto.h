/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "ApiModel.h"

class UserResourceDto final : public ApiModel
{
public:

    UserResourceDto(const SerializedData &tree) noexcept : ApiModel(tree) {}

    String getType() const noexcept { return DTO_PROPERTY(Resources::type); }
    String getName() const noexcept { return DTO_PROPERTY(Resources::name); }
    String getHash() const noexcept { return DTO_PROPERTY(Resources::hash); }
    Time getUpdateTime() const noexcept { return Time(DTO_PROPERTY(Resources::updatedAt)); }

    SerializedData getData() const noexcept { return DTO_CHILD(Resources::data); }

    JUCE_LEAK_DETECTOR(UserResourceDto)
};
