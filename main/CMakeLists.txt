
set(COM_RQUIRES mcommon mwifi mlink mdebug light_driver light_handle mesh_utils button_driver switch_driver buzzer_driver power_monitor)


set(COM_SRCS "switch.c")

idf_component_register(SRCS ${COM_SRCS}
                    INCLUDE_DIRS ""
                    REQUIRES ${COM_REQUIRES}
)
