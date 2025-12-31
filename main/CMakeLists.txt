idf_component_register(SRCS llm_main.c ai_agent.c rtc_proc.c audio_proc.c video_proc.c video_stub.c 
						 aic3104_ng.c i2s_fd_test.c gpio_test.c
					REQUIRES esp32-camera audio_hal audio_pipeline audio_stream audio_board esp_peripherals esp-adf-libs 
							 input_key_service esp_wifi nvs_flash agora_iot_sdk mbedtls)