//
// Created by thornhill on 08/05/25.
//

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP


#define PROTOCOL_VERSION "0.1.1"
#define PROTOCOL_VERSION_MAJOR      ((uint8_t) 0)
#define PROTOCOL_VERSION_MINOR      ((uint8_t) 1)
#define PROTOCOL_VERSION_PATCHLEVEL ((uint8_t) 1)


#define INSTRUCTION_MASK       (0xFF)
#define INSTRUCTION(x)         (x & INSTRUCTION_MASK)

#define CREATE                 (0x01)
#define READ                   (0x02)
#define UPDATE                 (0x03)
#define DELETE                 (0x04)
#define COMMAND                (0x05)


#define INSTANCE_MASK          (0xFF)
#define INSTANCE(x)            (x & INSTANCE_MASK)

#define INSTANCE_PROJECT       (0x1 << 0)
#define INSTANCE_LAYER         (0x1 << 1)
#define INSTANCE_OBJECT        (0x1 << 2)


#define CMD_MASK               (0xFF << 8)
#define CMD(x)                 (x & CMD_MASK)

#define CMD_STATUS             (0x1 << 0 << 8)
#define CMD_PROJECT            (0x1 << 1 << 8)
#define CMD_META               (0x1 << 2 << 8)


#define PROJECT_MASK           (0xFF)

#define PROJECT_START          (0x1 << 0)
#define PROJECT_STOP           (0x1 << 1)


#define META_MASK                  (0xFF)

#define META_LIST_LAYERS_TYPE      (0x1 << 0)
#define META_LIST_NODES_TYPE       (0x1 << 1)



#define STATUS_MASK            (0xFF)

#define STATUS_OK              (0x01)
#define STATUS_INTERNAL_ERROR  (0x02)
#define STATUS_INTERNAL_PANIC  (0x03)




#endif //PROTOCOL_HPP
