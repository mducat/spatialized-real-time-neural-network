//
// Created by thornhill on 08/05/25.
//

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

// 0.2.0
#define PROTOCOL_VERSION_MAJOR      ((uint8_t) 0)
#define PROTOCOL_VERSION_MINOR      ((uint8_t) 2)
#define PROTOCOL_VERSION_PATCHLEVEL ((uint8_t) 0)


#define STATUS_MASK            (0xFF)

#define STATUS_OK              (0x01)
#define STATUS_INTERNAL_ERROR  (0x02)
#define STATUS_INTERNAL_PANIC  (0x03)




#endif //PROTOCOL_HPP
