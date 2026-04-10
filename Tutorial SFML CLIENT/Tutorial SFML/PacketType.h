#pragma once

enum class PacketType
{
    NONE = 0,

    // Conexion inicial
    HANDSHAKE,

    // Login / Registro
    LOGIN_REQUEST,
    LOGIN_RESPONSE,

    REGISTER_REQUEST,
    REGISTER_RESPONSE,

    // Lobby
    CREATE_ROOM_REQUEST,
    CREATE_ROOM_RESPONSE,

    JOIN_ROOM_REQUEST,
    JOIN_ROOM_RESPONSE,

    ROOM_STATUS_UPDATE,
    START_GAME,

    // Generico
    ERROR_MESSAGE
};