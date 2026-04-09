#pragma once

enum class PacketType
{
    NONE = 0,

    // conexión/login
    HANDSHAKE,
    LOGIN_REQUEST,
    LOGIN_RESPONSE,
    REGISTER_REQUEST,
    REGISTER_RESPONSE,

    // lobby
    CREATE_ROOM_REQUEST,
    CREATE_ROOM_RESPONSE,

    JOIN_ROOM_REQUEST,
    JOIN_ROOM_RESPONSE,

    ROOM_STATUS_UPDATE,
    ROOM_PLAYER_JOINED,
    ROOM_PLAYER_LEFT,

    START_GAME,

    // utilidad
    ERROR_MESSAGE
};