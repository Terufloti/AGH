/// <reference types="vite/client" />
declare namespace NodeJS {
    interface ProcessEnv {
        VITE_API_BASE_URL: string;
    }
}