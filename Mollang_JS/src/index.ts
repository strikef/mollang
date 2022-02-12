import * as fs from "fs";
import { Command } from "commander";
import parse from "./parse";
import run from "./runtime";

async function main(code: string, parseOut: string) {
    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");

    const parsed = parse(code);
    if (parseOut) {
        fs.writeFile(
            parseOut,
            JSON.stringify(parsed),
            { encoding: "utf8" },
            (err) => {
                if (err) {
                    throw err;
                }
            }
        );
    }
    return run(...parsed);
}

const program = new Command();
program
    .name("mollang")
    .argument("<filename>")
    .option("-p, --parse <outfile>", "Write parsed data to file", false)
    .action(async (fn: string, options) => {
        if (fn.endsWith(".molu")) {
            if (fs.existsSync(fn)) {
                fs.readFile(fn, { encoding: "utf8" }, (err, data) => {
                    if (err) {
                        throw err;
                    } else {
                        main(data, options.parse);
                    }
                });
            } else {
                throw new Error("File not found");
            }
        } else {
            throw new Error("Unknown file type");
        }
    });

program.parse();
