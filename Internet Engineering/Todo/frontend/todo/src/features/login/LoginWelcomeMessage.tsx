import { useEffect, useState } from "react"
import { FC } from "react";
import { WelcomeMessageProps } from "../../types/WelcomeMessageProps";

export const WelcomeMessage: FC<WelcomeMessageProps> = ({ text, speed, delay }) => {
    const [displayedText, setDisplayedText] = useState("");
    const [index, setIndex] = useState(0);

    useEffect(() => {
        if(index < text.length) {
            const timeout = setTimeout(() => {
                setDisplayedText((prev) => prev + text[index]);
                setIndex((prev) => prev + 1);
            }, speed);

            return () => clearTimeout(timeout);
        } else {
            const resetTimeout = setTimeout(() => {
                setDisplayedText("");
                setIndex(0);
            }, delay);

            return () => clearTimeout(resetTimeout);
        }
    }, [index, text, speed, delay]);

    return <h2>{displayedText}</h2>
}