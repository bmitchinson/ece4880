import { h, Component } from 'preact';
import { Button } from 'preact-fluid';
import { auth, googleAuthProvider } from '../firebase/firebase';

import style from './controls.scss';

interface MyProps {}
interface MyState {
    currentUser: any;
}

const theme = {
    primaryColor: '#ffffff'
};

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            if (currentUser) {
                this.setState({ currentUser }, () => console.log('Logged In!'));
            } else {
                this.setState({ currentUser }, () => console.log('Logged Out!'));
            }
        });
    }

    render() {
        const user = this.state.currentUser;
        return (
            <div class={style.Stage}>
                {!user && (
                    <div class={style.Over}>
                        <h3>Sign in to access remote sensor controls</h3>
                        <Button class={style.Button} theme={theme} size={'large'} primary onClick={() => auth.signInWithRedirect(googleAuthProvider)}>
                            Sign In
                        </Button>
                    </div>
                )}
                {!user && <div class={style.Middle} />}
                {!user && (
                    <div class={style.Under}>
                        <h2>Controls Hidden</h2>
                    </div>
                )}
                {user && <h2>Controls Unlocked</h2>}
            </div>
        );
    }
}
